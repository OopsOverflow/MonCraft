#include "TerrainGenerator.hpp"

#include <glm/glm.hpp>
#include <stddef.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "multiplayer/terrain/ChunkGenerator.hpp"
#include "multiplayer/terrain/SliceMap.hpp"
#include "multiplayer/terrain/Structure.hpp"
#include "save/SaveManager.hpp"
#include "save/ServerConfig.hpp"
#include "terrain/AbstractChunk.hpp"
#include "terrain/ChunkMap.hpp"
#include "terrain/World.hpp"

using namespace glm;
using namespace std::chrono_literals;

// int distance(ivec3 a, ivec3 b) {
//   return std::max(std::max(abs(b.x - a.x), abs(b.y - a.y)), abs(b.z - a.z));
// }

TerrainGenerator::TerrainGenerator()
  : generating(false),
    generator(chunkSize),
    world(World::getInst())
{
  auto& config = Config::getServerConfig();
  threadCount = config.threadCount;
  startGeneration();
}

TerrainGenerator::~TerrainGenerator() {
  stopGeneration();
}

bool TerrainGenerator::sleepFor(std::chrono::milliseconds millis) {
  std::unique_lock<std::mutex> stopLck(stopMutex);
  return stopSignal.wait_for(stopLck, millis, [&]{return stopFlag;});
}

bool TerrainGenerator::addToBusyList(ivec3 cpos) {
  std::lock_guard<std::mutex> lck(busyListMutex);
  auto it = std::find(busyList.begin(), busyList.end(), cpos);
  if(it == busyList.end()) {
    busyList.push_back(cpos);
    return true;
  }
  else return false;
}

void TerrainGenerator::remFromBusyList(ivec3 cpos) {
  std::lock_guard<std::mutex> lck(busyListMutex);
  auto it = std::find(busyList.begin(), busyList.end(), cpos);
  if(it != busyList.end()) {
    busyList.erase(it);
  }
  else throw std::runtime_error("remFromBusyList failed");
}

std::shared_ptr<AbstractChunk> TerrainGenerator::getOrGen(ivec3 cpos) {
  static const auto sleep = 10ms;

  if(auto neigh = world.chunks.find(cpos)) {
    return neigh;
  }
  else if(addToBusyList(cpos)) {
      std::shared_ptr<AbstractChunk> chunk;
      std::unique_ptr<AbstractChunk> savedChunk = SaveManager::loadChunk(cpos);
      if (!savedChunk) {
        chunk = world.chunks.insert(cpos, generator.generate(cpos));
        sliceMap.insert(generator.generateStructures(*chunk));
      }
      else {
        chunk = world.chunks.insert(cpos, std::move(savedChunk));
      }

    remFromBusyList(cpos);
    return chunk;
  }
  else while(1) {
    sleepFor(sleep);
    if(auto neigh = world.chunks.find(cpos)) return neigh;
  }
}

void TerrainGenerator::setupNeighbors(std::shared_ptr<AbstractChunk> chunk) {
  for(size_t j = 0; j < 26; j++) {
    if(!chunk->neighbors[j].lock()) {
      ivec3 thisPos = chunk->chunkPos + AbstractChunk::neighborOffsets[j];
      if(auto neigh = world.chunks.find(thisPos)) {
        chunk->neighbors[j] = neigh;
        neigh->neighbors[AbstractChunk::neighborOffsetsInverse[j]] = chunk;
        if(neigh->hasAllNeighbors()) {
          computeChunk(neigh);
        }
      }
    }
  }

  if(chunk->hasAllNeighbors()) {
    computeChunk(chunk);
  }
}

void TerrainGenerator::computeChunk(std::shared_ptr<AbstractChunk> chunk) {
  auto slices = sliceMap.pop(chunk->chunkPos);
  for(auto const& slice : slices) {
    Structure::applySlice(*chunk, slice);
  }
  chunk->compute();
}

void TerrainGenerator::genWorker() {
  static const auto sleep = 1ms;
  static const auto longSleep = 500ms;
  bool stop;
  ivec3 cpos;

  do {
    if(waitingChunks.pop(cpos)) {
      auto chunk = getOrGen(cpos);

      setupNeighbors(chunk);

      stop = sleepFor(sleep);
    }
    else {
      stop = sleepFor(longSleep);
    }

  } while(!stop);
}

void TerrainGenerator::stopGeneration() {
  if(!generating) return;
  std::cout << "shutting down terrain thread..." << std::endl;
  {
    std::lock_guard<std::mutex> lk(stopMutex);
    stopFlag = true;
  }
  stopSignal.notify_all();
  for(auto& thread : genWorkerThreads) {
    thread.join();
  }
  std::cout << "terrain thread terminated" << std::endl;
  generating = false;
}

void TerrainGenerator::startGeneration() {
  if(generating) return;
  {
    std::lock_guard<std::mutex> lk(stopMutex);
    stopFlag = false;
  }
  std::cout << "starting generation" << std::endl;
  for(size_t i = 0; i < threadCount; i++) {
    genWorkerThreads.push_back(std::thread(&TerrainGenerator::genWorker, this));
  }
  generating = true;
}

void TerrainGenerator::toggleGeneration() {
  if(generating) stopGeneration();
  else startGeneration();
}
