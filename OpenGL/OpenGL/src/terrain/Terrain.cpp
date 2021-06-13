#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "Terrain.hpp"

using namespace glm;
using namespace std::chrono_literals;

int distance(ivec3 a, ivec3 b) {
  return std::max(std::max(abs(b.x - a.x), abs(b.y - a.y)), abs(b.z - a.z));
}

Terrain::Terrain()
  : generating(false),
    chunksMaxCount((2*renderDistH+2)*(2*renderDistH+2)*(2*renderDistV+2)),
    generator(chunkSize),
    chunkPos(0),
    chunkPosChanged(false)
{
  startGeneration();
}

Terrain::~Terrain() {
  stopGeneration();
}

void Terrain::updateWaitingList() {
  waitingChunks.clear();
  auto cpos = getChunkPos();

  auto insert = [&](ivec2 const& pos) {
    for(int i = -renderDistV; i <= renderDistV; i++) {
      ivec3 p = cpos + ivec3(pos.x, i, pos.y);
      auto chunk = chunkMap.find(p);
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

bool Terrain::sleepFor(std::chrono::milliseconds millis) {
  std::unique_lock<std::mutex> stopLck(stopMutex);
  return stopSignal.wait_for(stopLck, millis, [&]{return stopFlag;});
}

glm::ivec3 Terrain::getChunkPos() {
  std::lock_guard<std::mutex> lck(posMutex);
  return chunkPos;
}

void Terrain::setChunkPos(glm::ivec3 cpos) {
  // std::lock_guard<std::mutex> lck(posMutex);
  if(cpos != chunkPos) {
    chunkPos = cpos;
    chunkPosChanged = true;
  }
}

bool Terrain::hasPosChanged() {
  std::lock_guard<std::mutex> lck(posMutex);
  if(chunkPosChanged) {
    chunkPosChanged = false;
    return true;
  }
  return false;
}

void Terrain::mainWorker() {
  do {
    if(hasPosChanged()) updateWaitingList();
  } while(!sleepFor(100ms));
}

bool Terrain::addInBusyList(ivec3 cpos) {
  std::lock_guard<std::mutex> lck(busyListMutex);
  auto it = std::find(busyList.begin(), busyList.end(), cpos);
  if(it == busyList.end()) {
    busyList.push_back(cpos);
    return true;
  }
  else return false;
}

void Terrain::remFromBusyList(ivec3 cpos) {
  std::lock_guard<std::mutex> lck(busyListMutex);
  auto it = std::find(busyList.begin(), busyList.end(), cpos);
  if(it != busyList.end()) {
    busyList.erase(it);
  }
  else throw std::runtime_error("remFromBusyList failed");
}

void Terrain::genWorker() {
  auto sleep = 10ms;
  auto longSleep = 500ms;
  bool stop;

  auto getOrGen = [&](ivec3 cpos) {
    if(auto neigh = chunkMap.find(cpos)) {
      return neigh;
    }
    else if(addInBusyList(cpos)) {
      auto chunk = chunkMap.insert(cpos, generator.generate(cpos));
      sliceMap.insert(generator.generateStructures(cpos, *chunk));
      remFromBusyList(cpos);
      return chunk;
    }
    else while(1) {
      sleepFor(sleep);
      if(auto neigh = chunkMap.find(cpos)) return neigh;
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

      if(auto neigh = chunkMap.find(thisPos)) {
        neighbors[i] = neigh;
        finished[i] = true;
        count++;
      }
      else if(addInBusyList(thisPos)) {
        {
          auto chunk = chunkMap.insert(thisPos, generator.generate(thisPos));
          sliceMap.insert(generator.generateStructures(cpos, *chunk));
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

void Terrain::update(glm::vec3 pos) {
  ivec3 newChunkPos = floor(pos / float(chunkSize));
  setChunkPos(newChunkPos);

  // clear old chunks
  int delCount = std::max<int>(chunkMap.size() - chunksMaxCount, 0);
  chunkMap.eraseChunks(delCount, [newChunkPos](ivec3 cpos) {
    ivec3 dist = abs(newChunkPos - cpos);
    return dist.x > renderDistH + 1 || dist.z > renderDistH + 1 || dist.y > renderDistV + 1;
  });
}

void Terrain::render(Camera const& camera) {
  std::vector<std::pair<float, std::shared_ptr<Chunk>>> toRender;
  chunkMap.for_each([&](std::shared_ptr<Chunk> chunk) {
    vec3 worldChunkPos = vec3(chunk->chunkPos * chunkSize);
    vec3 chunkCenter = worldChunkPos + vec3(chunkSize) / 2.f;

    vec4 posCamSpace = camera.view * vec4(chunkCenter, 1.0f);
    static const float tolerance = chunkSize * .5f * sqrt(3.f);
    if(camera.chunkInView(posCamSpace,tolerance)) {
      toRender.emplace_back(-posCamSpace.z, chunk);
    }
  });

  std::sort(toRender.begin(), toRender.end(), [](auto& a, auto& b) {
    return a.first > b.first; // sort back-to-front (far chunks first)
  });

  for (auto iter = toRender.rbegin(); iter != toRender.rend(); ++iter) {
    auto& pair = *iter;
    pair.second->update();
    pair.second->drawSolid();
  }
  auto viewDir = camera.center - camera.position;
  for(auto& pair : toRender) {
    pair.second->drawTransparent(viewDir);
  }

  // generator.biomeSampler.testtex.draw();
}

Block* Terrain::getBlock(ivec3 pos) {
  ivec3 cpos = floor(vec3(pos) / float(chunkSize));

  if(auto chunk = chunkMap.find(cpos)) {
    ivec3 dpos = pos - cpos * chunkSize;
    return chunk->at(dpos).get();
  }

  return nullptr;
}

void Terrain::setBlock(ivec3 pos, Block::unique_ptr_t block) {
  ivec3 cpos = floor(vec3(pos) / float(chunkSize));

  auto chunk = chunkMap.find(cpos);
  if(!chunk) throw std::runtime_error("setBlock: chunk not found");

  ivec3 dpos = pos - cpos * chunkSize;
  chunk->setBlock(dpos, std::move(block));
}

void Terrain::stopGeneration() {
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

void Terrain::startGeneration() {
  if(generating) return;
  {
    std::lock_guard<std::mutex> lk(stopMutex);
    stopFlag = false;
  }
  std::cout << "starting generation" << std::endl;
  mainWorkerThread = std::thread(&Terrain::mainWorker, this);
  for(auto& thread : genWorkerThreads) {
    thread = std::thread(&Terrain::genWorker, this);
  }
  generating = true;
}

void Terrain::toggleGeneration() {
  if(generating) stopGeneration();
  else startGeneration();
}
