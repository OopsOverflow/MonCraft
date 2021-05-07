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

  auto insert = [&](ivec2 const& pos) {
    std::lock_guard<std::mutex> lck(chunksMutex);
    for(int i = -renderDistV; i <= renderDistV; i++) {
      ivec3 p = chunkPos + ivec3(pos.x, i, pos.y);
      auto it = chunks.find(p);
      if(it == chunks.end() || !it->second->isLoaded()) {
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

void Terrain::mainWorker() {
  auto longSleep = 100ms;
  bool stop;

  do {

    bool changed = false;
    {
      std::lock_guard<std::mutex> lck(chunksMutex);
      changed = chunkPosChanged;
      chunkPosChanged = false;
    }

    if(changed) {
      updateWaitingList();
    }

    {std::unique_lock<std::mutex> stopLck(stopMutex);
    stop = stopSignal.wait_for(stopLck, longSleep, [&]{return stopFlag;});}

  } while(!stop);
}

// gets the chunks in the hashmap, or safely generates, stores then returns them.
std::vector<std::shared_ptr<Chunk>> Terrain::getOrGenAllChunks(glm::ivec3 center, std::vector<glm::ivec3> const& offsets) {
  std::vector<std::shared_ptr<Chunk>> res(offsets.size());
  std::vector<std::pair<int, std::shared_future<std::shared_ptr<Chunk>>>> work;

  for(size_t i = 0; i < offsets.size(); i++) {
    auto pos = center + offsets[i];
    bool ok = false;

    // look for pos in wip
    if(!ok) {
      std::lock_guard<std::mutex> lck(wipMutex);
      auto it = std::find_if(wip.begin(), wip.end(), [pos](auto const& pair) {
        return pair.first == pos;
      });
      if(it != wip.end()) {
        work.emplace_back(i, it->second);
        ok = true;
      }
    }

    // else, look for pos in chunks
    if(!ok) {
      std::lock_guard<std::mutex> lck(chunksMutex);
      auto it = chunks.find(pos);
      if(it != chunks.end()) {
        res[i] = it->second;
        ok = true;
      }
    }

    // else, generate
    if(!ok) {
      std::promise<std::shared_ptr<Chunk>> prom;
      {
        std::lock_guard<std::mutex> lck(wipMutex);
        wip.emplace_back(pos, prom.get_future().share());
      }
      auto chunk = generator.generate(pos);
      res[i] = chunk;
      prom.set_value(chunk);
      {
        std::lock_guard<std::mutex> lck(chunksMutex);
        chunks.emplace(pos, chunk);
      }
      {
        std::lock_guard<std::mutex> lck(wipMutex);
        auto it = std::find_if(wip.begin(), wip.end(), [pos](auto const& pair) {
          return pair.first == pos;
        });
        if(it != wip.end()) {
          wip.erase(it);
        }
      }
    }
  }

  // now, wait for all futures
  for(auto const& task : work) {
    res[task.first] = task.second.get();
  }

  return res;
}

void Terrain::genWorker() {
  auto sleep = 10ms;
  auto longSleep = 500ms;
  bool stop;

  do {

    ivec3 pos;
    if(waitingChunks.pop(pos)) {

      std::vector<ivec3> offsets(27);
      offsets[0] = ivec3(0);
      std::copy(Chunk::neighborOffsets.begin(), Chunk::neighborOffsets.end(), ++offsets.begin());
      auto chunks = getOrGenAllChunks(pos, offsets);
      {
        std::lock_guard<std::mutex> lck(chunksMutex);
        auto const& chunk = chunks[0];
        std::copy(++chunks.begin(), chunks.end(), chunk->neighbors.begin());
        for(size_t i = 1; i < chunks.size(); i++) {
          int inv = Chunk::neighborOffsetsInverse[i-1];
          chunks[i]->neighbors[inv] = chunk;
        }
      }
      chunks[0]->compute();

      {std::unique_lock<std::mutex> stopLck(stopMutex);
      stop = stopSignal.wait_for(stopLck, sleep, [&]{return stopFlag;});}
    }
    else {
      // std::cout << "worker idle" << std::endl;
      {std::unique_lock<std::mutex> stopLck(stopMutex);
      stop = stopSignal.wait_for(stopLck, longSleep, [&]{return stopFlag;});}
    }

  } while(!stop);
}

void Terrain::update(glm::vec3 pos, glm::vec3 dir, float fovX) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  playerPos = pos;
  viewDir = glm::normalize(dir);
  this->fovX = fovX;

  ivec3 newChunkPos = floor(pos / float(chunkSize));
  if(newChunkPos != chunkPos) {
    chunkPos = newChunkPos;
    chunkPosChanged = true;
  }

  // clear old chunks
  int count = (int)chunks.size();
  for(auto it = chunks.begin(); it != chunks.end() && count > chunksMaxCount;) {
    ivec3 const& a = it->first;
    ivec3 const& b = chunkPos;

    if(abs(b.x - a.x) > renderDistH || abs(b.z - a.z) > renderDistH || abs(b.y - a.y) > renderDistV) {
      it = chunks.erase(it);
      count --;
    }
    else {
      ++it;
    }
  }
}

void Terrain::render() {
  std::lock_guard<std::mutex> lck(chunksMutex);

  vec2 viewDir2D = glm::normalize(vec2(viewDir.x, viewDir.z));
  ivec3 startChunk = chunkPos - ivec3(glm::sign(viewDir));

  for(auto& chunk : chunks) {
    ivec3 chunkDir3D = chunk.first - startChunk;
    vec2 chunkDir = glm::normalize(vec2(chunkDir3D.x, chunkDir3D.z));

    float minDot = cos(glm::radians(fovX));
    if(distance(chunk.first, chunkPos) < 2 || glm::dot(chunkDir, viewDir2D) > minDot) {
      chunk.second->drawSolid();
    }
  }
}

Block* Terrain::getBlock(ivec3 pos) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  ivec3 cpos = floor(vec3(pos) / float(chunkSize));

  if(chunks.find(cpos) == chunks.end())
    return nullptr;

  ivec3 dpos = pos - cpos * chunkSize;
  return chunks.at(cpos)->at(dpos).get();
}

void Terrain::setBlock(ivec3 pos, Block::unique_ptr_t block) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  ivec3 cpos = floor(vec3(pos) / float(chunkSize));

  auto it = chunks.find(cpos);
  if(it == chunks.end())
    throw std::runtime_error("setBlock: chunk not found");

  ivec3 dpos = pos - cpos * chunkSize;

  auto chunk = it->second;
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
