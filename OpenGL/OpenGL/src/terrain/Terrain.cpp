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

int distance(ivec3 a, ivec3 b) {
  return std::max(std::max(abs(b.x - a.x), abs(b.y - a.y)), abs(b.z - a.z));
}

Terrain::Terrain()
  : generator(chunkSize),
    chunkPos(0),
    loader(),
    texture(loader.loadTexture("Texture_atlas"))
{
  workerThread = std::thread(&Terrain::worker, this, stopTrigger.get_future());
}

Terrain::~Terrain() {
  std::cout << "shutting down terrain thread..." << std::endl;
  stopTrigger.set_value();
  workerThread.join();
  std::cout << "terrain thread terminated" << std::endl;
}

#include "Debug.hpp"

// COMBAK: this function is weird and could be improved.
bool Terrain::getNextPos(ivec3 &pos) {
  std::lock_guard<std::mutex> lck2(chunksMutex);

  ivec3 iter(-renderDistance, 0, -1);

  auto tryPos = [&](ivec3 dpos) {
    pos = chunkPos + dpos;
    if (chunks.find(pos) == chunks.end()) {
      lastIter = iter;
      return true;
    }
    return false;
  };

  if(chunkPos == lastPos) {
    iter = lastIter;
  } else {
    lastPos = chunkPos;
  }
  if(iter.x == 0) {
    if (tryPos(ivec3(0))) return true;
    iter.x = 1;
  }

  for(; iter.x <= renderDistance; iter.x++) {
    for(; iter.z <= iter.x; iter.z++) {
      for(; iter.y < renderDistance; iter.y++) {
        if(tryPos({iter.x, iter.y, iter.z}))  return true;
        if(tryPos({-iter.x, iter.y, iter.z})) return true;
        if(tryPos({iter.z, iter.y, iter.x}))  return true;
        if(tryPos({iter.z, iter.y, -iter.x})) return true;
      }
      iter.y = -renderDistance;
    }
    iter.z = -iter.x - 1;
  }
  return false;
}

void Terrain::worker(std::future<void> stopSignal) {
  auto sleep = 1ms;
  auto longSleep = 100ms;
  std::future_status stop;

  ivec3 pos;

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
      std::cout << "worker idle" << std::endl;
      stop = stopSignal.wait_for(longSleep);
    }
    if(stop != std::future_status::timeout) return; // stopSignal was triggered
  }
}

void Terrain::update(glm::vec3 pos, glm::vec3 dir, float fovX) {
  std::lock_guard<std::mutex> lck(chunksMutex);
  playerPos = pos;
  viewDir = glm::normalize(dir);
  chunkPos = floor(pos / float(chunkSize));
  this->fovX = fovX;

  // clear old chunks
  int count = (int)chunks.size();
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
  return chunks.at(cpos)->getBlock(dpos + ivec3(1));
}
