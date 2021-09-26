#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "TerrainGenerator.hpp"

using namespace glm;
using namespace std::chrono_literals;

// int distance(ivec3 a, ivec3 b) {
//   return std::max(std::max(abs(b.x - a.x), abs(b.y - a.y)), abs(b.z - a.z));
// }

TerrainGenerator::TerrainGenerator()
  : generating(false),
    generator(chunkSize),
    save("save/serverWorld/chunk"),
    world(World::getInst())
{
  startGeneration();
}

TerrainGenerator::~TerrainGenerator() {
  stopGeneration();
}

// void TerrainGenerator::updateWaitingList() {
//   waitingChunks.clear();
//   auto cpos = getChunkPos();
//
//   auto insert = [&](ivec2 const& pos) {
//     for(int i = -renderDistV; i <= renderDistV; i++) {
//       ivec3 p = cpos + ivec3(pos.x, i, pos.y);
//       auto chunk = world.chunks.find(p);
//       if(!chunk || !chunk->isLoaded()) {
//         waitingChunks.push(p);
//       }
//     }
//   };
//
//   insert({0, 0});
//
//   for(int dist = 1; dist < renderDistH+1; dist++) {
//     insert({dist, 0});
//     insert({0, dist});
//     insert({-dist, 0});
//     insert({0, -dist});
//
//     for(int off = 1; off < dist; off++) {
//       insert({dist, off});
//       insert({off, dist});
//       insert({-off, dist});
//       insert({-dist, off});
//       insert({-dist, -off});
//       insert({-off, -dist});
//       insert({off, -dist});
//       insert({dist, -off});
//     }
//
//     insert({dist, dist});
//     insert({-dist, dist});
//     insert({-dist, -dist});
//     insert({dist, -dist});
//   }
// }

bool TerrainGenerator::sleepFor(std::chrono::milliseconds millis) {
  std::unique_lock<std::mutex> stopLck(stopMutex);
  return stopSignal.wait_for(stopLck, millis, [&]{return stopFlag;});
}

// glm::ivec3 TerrainGenerator::getChunkPos() {
//   std::lock_guard<std::mutex> lck(posMutex);
//   return chunkPos;
// }
//
// void TerrainGenerator::setChunkPos(glm::ivec3 cpos) {
//   std::lock_guard<std::mutex> lck(posMutex);
//   if(cpos != chunkPos) {
//     chunkPos = cpos;
//     chunkPosChanged = true;
//   }
// }
//
// bool TerrainGenerator::hasPosChanged() {
//   std::lock_guard<std::mutex> lck(posMutex);
//   if(chunkPosChanged) {
//     chunkPosChanged = false;
//     return true;
//   }
//   return false;
// }

// void TerrainGenerator::mainWorker() {
//   updateWaitingList();
//   do {
//     if(hasPosChanged()) updateWaitingList();
//   } while(!sleepFor(100ms));
// }

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

std::shared_ptr<Chunk> TerrainGenerator::getOrGen(ivec3 cpos) {
  static const auto sleep = 10ms;

  if(auto neigh = world.chunks.find(cpos)) {
    return neigh;
  }
  else if(addToBusyList(cpos)) {
      std::shared_ptr<Chunk> chunk;
      std::unique_ptr<Chunk> savedChunk = save.getChunk(cpos);
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

void TerrainGenerator::setupNeighbors(std::shared_ptr<Chunk> chunk) {
  for(size_t j = 0; j < 26; j++) {
    if(!chunk->neighbors[j].lock()) {
      ivec3 thisPos = chunk->chunkPos + Chunk::neighborOffsets[j];
      if(auto neigh = world.chunks.find(thisPos)) {
        chunk->neighbors[j] = neigh;
        neigh->neighbors[Chunk::neighborOffsetsInverse[j]] = chunk;
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

void TerrainGenerator::computeChunk(std::shared_ptr<Chunk> chunk) {
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
  for(auto& thread : genWorkerThreads) {
    thread = std::thread(&TerrainGenerator::genWorker, this);
  }
  generating = true;
}

void TerrainGenerator::toggleGeneration() {
  if(generating) stopGeneration();
  else startGeneration();
}
