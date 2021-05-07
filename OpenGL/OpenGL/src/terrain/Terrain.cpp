#include "Terrain.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

using glm::ivec2;
using glm::ivec3;
using glm::vec3;
using glm::vec2;
using namespace std::chrono_literals;

int distance(ivec3 a, ivec3 b) {
  return std::max(std::max(abs(b.x - a.x), abs(b.y - a.y)), abs(b.z - a.z));
}

Terrain::Terrain()
  : chunksMaxCount((2*renderDistH+2)*(2*renderDistH+2)*(2*renderDistV+2)),
    generator(chunkSize),
    chunkPos(0),
    chunkPosChanged(false),
    loader(),
    texture(loader.loadTexture("Texture_atlas"))
{
  mainWorkerThread = std::thread(&Terrain::mainWorker, this);
  for(auto& thread : genWorkerThreads) {
    thread = std::thread(&Terrain::genWorker, this);
  }
}

Terrain::~Terrain() {
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
}

#include "debug/Debug.hpp"

void Terrain::updateWaitingList() {
  waitingChunks.clear();
  auto cpos = getChunkPos();

  auto insert = [&](ivec2 const& pos) {
    for(int i = -renderDistV; i <= renderDistV; i++) {
      ivec3 p = cpos + ivec3(pos.x, i, pos.y);
      auto chunk = chunks.find(p);
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
  std::lock_guard<std::mutex> lck(posMutex);
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
    if(auto neigh = chunks.find(cpos)) {
      return neigh;
    }
    else if(addInBusyList(cpos)) {
      chunks.insert(cpos, generator.generate(cpos));
      remFromBusyList(cpos);
      return chunks.find(cpos);
    }
    else while(1) {
      sleepFor(sleep);
      if(auto neigh = chunks.find(cpos)) return neigh;
    }
  };

  auto getOrGenN = [&](ivec3 cpos) {
    int count = 0;
    std::array<bool, 26> finished;
    std::array<std::weak_ptr<Chunk>, 26> neighbors;
    int i = 0;

    while(count < 26) {
     if(finished[i]) continue;
      ivec3 thisPos = cpos + Chunk::neighborOffsets[i];

      if(auto neigh = chunks.find(thisPos)) {
        neighbors[i] = neigh;
        finished[i] = true;
        count++;
      }
      else if(addInBusyList(cpos)) {
        chunks.insert(cpos, generator.generate(cpos));
        remFromBusyList(cpos);
        sleepFor(sleep);
        neighbors[i] = chunks.find(cpos);
        finished[i] = true;
        count++;
      }
      i = (i + 1) % 26;
    }
  };

  do {

    ivec3 pos;
    if(waitingChunks.pop(pos)) {

      auto chunk = getOrGen(pos);

      std::array<bool, 26> success;
      std::array<std::weak_ptr<Chunk>, 26> neighbors;

      for(size_t i = 0; i < 26; i++) {
        auto neigh = getOrGen(pos + Chunk::neighborOffsets[i]);
        neigh->neighbors[Chunk::neighborOffsetsInverse[i]] = chunk;
        neighbors[i] = neigh;
      }

      chunk->neighbors = neighbors;
      chunk->compute();

      stop = sleepFor(sleep);
    }
    else {
      stop = sleepFor(longSleep);
    }

  } while(!stop);
}

void Terrain::update(glm::vec3 pos, glm::vec3 dir, float fovX) {
  playerPos = pos;
  viewDir = glm::normalize(dir);
  this->fovX = fovX;

  ivec3 newChunkPos = floor(pos / float(chunkSize));
  setChunkPos(newChunkPos);

  // clear old chunks
  chunks.eraseChunks(10, [newChunkPos](ivec3 cpos) {
    ivec3 const& a = cpos;
    ivec3 const& b = newChunkPos;
    return abs(b.x - a.x) > renderDistH + 1 || abs(b.z - a.z) > renderDistH + 1 || abs(b.y - a.y) > renderDistV + 1;
  });
}

void Terrain::render() {
  ivec3 cpos = getChunkPos();
  vec2 viewDir2D = glm::normalize(vec2(viewDir.x, viewDir.z));
  ivec3 startChunk = cpos - ivec3(glm::sign(viewDir));

  // draw solid and update chunks
  for(auto& chunk : chunks) {
    ivec3 chunkDir3D = chunk.first - startChunk;
    vec2 chunkDir = glm::normalize(vec2(chunkDir3D.x, chunkDir3D.z));

    float minDot = cos(glm::radians(fovX));
    if(distance(chunk.first, cpos) < 2 || glm::dot(chunkDir, viewDir2D) > minDot) {
      chunk.second->update();
      chunk.second->drawSolid();
    }
  }

  // draw transparent
  // glDisable(GL_CULL_FACE);
  for(auto& chunk : chunks) {
    ivec3 chunkDir3D = chunk.first - startChunk;
    vec2 chunkDir = glm::normalize(vec2(chunkDir3D.x, chunkDir3D.z));

    float minDot = cos(glm::radians(fovX));
    if(distance(chunk.first, cpos) < 2 || glm::dot(chunkDir, viewDir2D) > minDot) {
      chunk.second->drawTransparent();
    }
  }
  // glEnable(GL_CULL_FACE);
}

Block* Terrain::getBlock(ivec3 pos) {
  ivec3 cpos = floor(vec3(pos) / float(chunkSize));

  if(auto chunk = chunks.find(cpos)) {
    ivec3 dpos = pos - cpos * chunkSize;
    return chunk->at(dpos).get();
  }

  return nullptr;
}

void Terrain::setBlock(ivec3 pos, Block::unique_ptr_t block) {
  ivec3 cpos = floor(vec3(pos) / float(chunkSize));

  auto chunk = chunks.find(cpos);
  if(!chunk) throw std::runtime_error("setBlock: chunk not found");

  ivec3 dpos = pos - cpos * chunkSize;

  chunk->at(dpos) = std::move(block);
  chunk->compute();

  // now update neighbors if close to a border
  // COMBAK: this seems overly compicated and may be moved to Chunk.
  static const ivec3 mask(9, 3, 1);

  auto greater = equal(dpos, ivec3(chunkSize - 1));
  auto lesser = equal(dpos, ivec3(0));

  if(any(greater) || any(lesser)) {
    auto tmp = ivec3(greater) * mask + ivec3(lesser) * 2 * mask;

    std::vector<int> updates;
    for(int i = 0; i < 2*2*2; i++) {
      int index = tmp.x * ((i&0b100)>>2) + tmp.y * ((i&0b010)>>1) + tmp.z * (i&0b001);
      if(index == 0) continue;
      if(std::find(updates.begin(), updates.end(), index) != updates.end()) continue;
      updates.push_back(index);
      if(auto neigh = chunk->neighbors[index - 1].lock()) {
        neigh->compute();
      }
    }
  }

}
