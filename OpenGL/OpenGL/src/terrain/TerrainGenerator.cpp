#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "TerrainGenerator.hpp"

using namespace glm;
using namespace std::chrono_literals;

int distance(ivec3 a, ivec3 b) {
  return std::max(std::max(abs(b.x - a.x), abs(b.y - a.y)), abs(b.z - a.z));
}

TerrainGenerator::TerrainGenerator()
  : generating(false),
    chunksMaxCount((2*renderDistH+2)*(2*renderDistH+2)*(2*renderDistV+2)),
    generator(chunkSize),
    chunkPos(0),
    chunkPosChanged(false),
    world(World::getInst()),
    save("save/defaultWorld/chunk")
{
  startGeneration();
}

TerrainGenerator::~TerrainGenerator() {
  stopGeneration();
}

void TerrainGenerator::updateWaitingList() {
  waitingChunks.clear();
  auto cpos = getChunkPos();

  auto insert = [&](ivec2 const& pos) {
    for(int i = -renderDistV; i <= renderDistV; i++) {
      ivec3 p = cpos + ivec3(pos.x, i, pos.y);
      auto chunk = world.chunks.find(p);
      if(!chunk || !chunk->isLoaded()) {
        waitingChunks.push(p);
      }
    }
  };

  insert({0, 0});

  for(int dist = 1; dist < renderDistH+1; dist++) {
    insert({dist, 0});
    insert({0, dist});
    insert({-dist, 0});
    insert({0, -dist});

    for(int off = 1; off < dist; off++) {
      insert({dist, off});
      insert({off, dist});
      insert({-off, dist});
      insert({-dist, off});
      insert({-dist, -off});
      insert({-off, -dist});
      insert({off, -dist});
      insert({dist, -off});
    }

    insert({dist, dist});
    insert({-dist, dist});
    insert({-dist, -dist});
    insert({dist, -dist});
  }
}

bool TerrainGenerator::sleepFor(std::chrono::milliseconds millis) {
  std::unique_lock<std::mutex> stopLck(stopMutex);
  return stopSignal.wait_for(stopLck, millis, [&]{return stopFlag;});
}

glm::ivec3 TerrainGenerator::getChunkPos() {
  std::lock_guard<std::mutex> lck(posMutex);
  return chunkPos;
}

void TerrainGenerator::setChunkPos(glm::ivec3 cpos) {
  std::lock_guard<std::mutex> lck(posMutex);
  if(cpos != chunkPos) {
    chunkPos = cpos;
    chunkPosChanged = true;
  }
}

bool TerrainGenerator::hasPosChanged() {
  std::lock_guard<std::mutex> lck(posMutex);
  if(chunkPosChanged) {
    chunkPosChanged = false;
    return true;
  }
  return false;
}

void TerrainGenerator::mainWorker() {
  do {
    if(hasPosChanged()) updateWaitingList();
  } while(!sleepFor(100ms));
}

bool TerrainGenerator::addInBusyList(ivec3 cpos) {
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

void TerrainGenerator::genWorker() {
  auto sleep = 10ms;
  auto longSleep = 500ms;
  bool stop;

  auto getOrGen = [&](ivec3 cpos) {
    if(auto neigh = world.chunks.find(cpos)) {
      return neigh;
    }
    else if(addInBusyList(cpos)) {
        std::shared_ptr<Chunk> chunk;
        std::unique_ptr<Chunk> savedChunk = save.getChunk(cpos);
        if (!savedChunk) {
            chunk = world.chunks.insert(cpos, generator.generate(cpos));
            sliceMap.insert(generator.generateStructures(*chunk));
            save.saveChunk(chunk);
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
  };

  auto getOrGenN = [&](ivec3 cpos) {
    int count = 0;
    std::array<bool, 26> finished{};
    for(int i = 0; i < 26; i++) finished[i] = false;
    std::array<std::weak_ptr<Chunk>, 26> neighbors;
    int i = 0;

    while(count < 26) {
      while(finished[i]) i = (i + 1) % 26;
      ivec3 thisPos = cpos + Chunk::neighborOffsets[i];

      if(auto neigh = world.chunks.find(thisPos)) {
        neighbors[i] = neigh;
        finished[i] = true;
        count++;
      }
      else if(addInBusyList(thisPos)) {
        {
              std::shared_ptr<Chunk> chunk;
              std::unique_ptr<Chunk> savedChunk = save.getChunk(thisPos);
              if (!savedChunk) {
                  chunk = world.chunks.insert(thisPos, generator.generate(thisPos));
                  sliceMap.insert(generator.generateStructures(*chunk));
                  save.saveChunk(chunk);
              }
              else {
                  chunk = world.chunks.insert(thisPos, std::move(savedChunk));
                  //std::cout << cpos.x << " " << cpos.y << " " << cpos.z << " " << chunk->chunkPos.x << " " << chunk->chunkPos.y << " " << chunk->chunkPos.z << std::endl;
              }

          remFromBusyList(thisPos);
          neighbors[i] = chunk;
          finished[i] = true;
          count++;
        }
      }
      else sleepFor(sleep);
    }

    return neighbors;
  };

  do {

    ivec3 cpos;
    if(waitingChunks.pop(cpos)) {

      auto chunk = getOrGen(cpos);

      chunk->neighbors = getOrGenN(cpos);
      for(size_t i = 0; i < 26; i++) {
        if(auto neigh = chunk->neighbors[i].lock()) {
          neigh->neighbors[Chunk::neighborOffsetsInverse[i]] = chunk;
        }
      }

      auto slices = sliceMap.pop(cpos);
      for(auto const& slice : slices) {
        Structure::applySlice(*chunk, slice);
      }

      chunk->compute();

      stop = sleepFor(sleep);
    }
    else {
      stop = sleepFor(longSleep);
    }

  } while(!stop);
}

void TerrainGenerator::update(glm::vec3 pos) {
  ivec3 newChunkPos = floor(pos / float(chunkSize));
  setChunkPos(newChunkPos);

  // clear old chunks
  int delCount = std::max<int>(world.chunks.size() - chunksMaxCount, 0);
  world.chunks.eraseChunks(delCount, [newChunkPos](ivec3 cpos) {
    ivec3 dist = abs(newChunkPos - cpos);
    return dist.x > renderDistH + 1 || dist.z > renderDistH + 1 || dist.y > renderDistV + 1;
  });
}

void TerrainGenerator::stopGeneration() {
  if(!generating) return;
  std::cout << "shutting down terrain thread..." << std::endl;
  {
    std::lock_guard<std::mutex> lk(stopMutex);
    stopFlag = true;
  }
  stopSignal.notify_all();
  mainWorkerThread.join();
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
  mainWorkerThread = std::thread(&TerrainGenerator::mainWorker, this);
  for(auto& thread : genWorkerThreads) {
    thread = std::thread(&TerrainGenerator::genWorker, this);
  }
  generating = true;
}

void TerrainGenerator::toggleGeneration() {
  if(generating) stopGeneration();
  else startGeneration();
}
