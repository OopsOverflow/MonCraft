#include "Terrain.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

using glm::ivec2;
using glm::ivec3;
using glm::vec3;
using glm::vec2;
using namespace std::chrono_literals;

// the distance function may be changed in the future.
int distance(ivec2 a, ivec2 b) {
  return std::max(abs(b.x - a.x), abs(b.y - a.y));
}

Terrain::Terrain()
  : generator(chunkSize),
    chunkPos(0)
{
  workerThread = std::thread(&Terrain::worker, this, stopTrigger.get_future());
}

Terrain::~Terrain() {
  std::cout << "shutting down terrain thread..." << std::endl;
  stopTrigger.set_value();
  workerThread.join();
  std::cout << "terrain thread terminated" << std::endl;
}

// COMBAK: this function is weird and could be improved.
bool Terrain::getNextPos(ivec2 &pos) {
  std::lock_guard<std::mutex> lck2(chunksMutex);

  int i = 0, j = -1;

  auto tryPos = [&](int k, int l) {
    pos = chunkPos + ivec2(k, l);
    if (chunks.find(pos) == chunks.end()) {
      last_i = i;
      last_j = j;
      return true;
    }
    return false;
  };

  if(chunkPos == lastPos) {
    i = last_i;
    j = last_j;
  } else {
    lastPos = chunkPos;
  }
  if(i == 0) {
    if (tryPos(0, 0)) return true;
    i++;
  }

  for(; i <= renderDistance; i++) {
    for(; j <= i; j++) {
      if (tryPos(i, j))  return true;
      if (tryPos(-i, j)) return true;
      if (tryPos(j, i))  return true;
      if (tryPos(j, -i)) return true;
    }
    j = -i - 1;
  }
  return false;
}

void Terrain::worker(std::future<void> stopSignal) {
  auto sleep = 1ms;
  auto longSleep = 100ms;
  std::future_status stop;

  ivec2 pos;

  while(1) {
    if(getNextPos(pos)) {
      {
        Chunk* c = generator.generate(pos);
        {
          std::lock_guard<std::mutex> lock(chunksMutex);
          chunks.emplace(pos, c);
        }
      }
      stop = stopSignal.wait_for(sleep);
    }
    else {
      // std::cout << "worker idle" << std::endl;
      stop = stopSignal.wait_for(longSleep);
    }
    if(stop != std::future_status::timeout) return; // stopSignal was triggered
  }
}

void Terrain::update(glm::vec3 pos) {
  std::lock_guard<std::mutex> lck2(chunksMutex);
  chunkPos = floor(vec2(pos.x, pos.z) / float(chunkSize));

  // clear old chunks
  int count = (int)chunks.size();
  if(count > chunksMaxCount) {
    for (auto it = chunks.begin(); it != chunks.end() && count > chunksMaxCount;) {
      int dist = distance(it->first, chunkPos);
      if (dist > renderDistance) {
        it = chunks.erase(it);
        count --;
      }
      else
      ++it;
    }
  }
}

void Terrain::render() {
  std::lock_guard<std::mutex> lck2(chunksMutex);

  for(auto& chunk : chunks) {
    Mesh const& mesh = chunk.second->getMesh();
    glBindVertexArray(mesh.getVAO());
    glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(mesh.model));
    glDrawElements(GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_INT, nullptr);
  }
}

Block* Terrain::getBlock(ivec3 pos) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  ivec2 cpos = floor(vec2(pos.x, pos.z) / float(chunkSize));
  if(pos.y < 0 || pos.y >= chunkSize) // TODO: change this once the terrain height is established
    return nullptr;
  if(chunks.find(cpos) == chunks.end())
    return nullptr;

  ivec3 dpos = pos - ivec3(cpos.x, 0, cpos.y) * chunkSize;
  return chunks.at(cpos)->getBlock(dpos + ivec3(1));
}
