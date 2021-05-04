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
  : chunksMaxCount((2*renderDistH+1)*(2*renderDistH+1)*(2*renderDistV+1)),
    generator(chunkSize),
    chunkPos(0),
    chunkPosChanged(false),
    loader(),
    texture(loader.loadTexture("Texture_atlas"))
{
  int n = 0;
  for(auto& thread : workerThreads) {
    thread = std::thread(&Terrain::worker, this, n);
    n++;
  }
}

Terrain::~Terrain() {
  std::cout << "shutting down terrain thread..." << std::endl;
  {
    std::lock_guard<std::mutex> lk(stopMutex);
    stopFlag = true;
  }
  stopSignal.notify_all();
  for(auto& thread : workerThreads) {
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
      if(chunks.find(p) == chunks.end()) {
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

void Terrain::worker(int n) {
  auto sleep = 1ms;
  auto longSleep = 500ms;
  bool stop;

  do {

    if(n == 0) {
      bool changed = false;
      {
        std::lock_guard<std::mutex> lck(chunksMutex);
        changed = chunkPosChanged;
        chunkPosChanged = false;
      }

      if(changed) {
        updateWaitingList();
      }
    }

    ivec3 pos;
    if(waitingChunks.pop(pos)) {
      std::shared_ptr<Chunk> c(generator.generate(pos));
      {
        std::lock_guard<std::mutex> lck(chunksMutex);
        chunks.emplace(pos, c);
      }

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
    Mesh const& mesh = chunk.second->getMesh();
    ivec3 chunkDir3D = chunk.first - startChunk;
    vec2 chunkDir = glm::normalize(vec2(chunkDir3D.x, chunkDir3D.z));

    float minDot = cos(glm::radians(fovX));

    if(distance(chunk.first, chunkPos) < 2 || glm::dot(chunkDir, viewDir2D) > minDot) {
      glBindVertexArray(mesh.getVAO());
      glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(mesh.model));
      glDrawElements(GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_INT, nullptr);
    }
  }
}

Block* Terrain::getBlock(ivec3 pos) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  ivec3 cpos = floor(vec3(pos) / float(chunkSize));

  if(chunks.find(cpos) == chunks.end())
    return nullptr;

  ivec3 dpos = pos - cpos * chunkSize;
  return chunks.at(cpos)->getBlock(dpos);
}

void Terrain::setBlock(ivec3 pos, Block::unique_ptr_t block) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  ivec3 cpos = floor(vec3(pos) / float(chunkSize));

  if(chunks.find(cpos) == chunks.end())
    throw std::runtime_error("setBlock: chunk not found");

  ivec3 dpos = pos - cpos * chunkSize;
  return chunks.at(cpos)->setBlock(dpos, std::move(block));
}
