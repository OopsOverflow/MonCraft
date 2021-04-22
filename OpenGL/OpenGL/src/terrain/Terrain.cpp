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
  : chunksMaxCount((int)pow(renderDistance * 2 + 1, 3)),
    chunkMemorySize(sizeof(nullptr) * pow(chunkSize + 2, 3) / 1024),
    chunkCacheSize(memoryCap * 1024 / chunkMemorySize),
    generator(chunkSize),
    chunkPos(0),
    chunkPosChanged(false),
    loader(),
    texture(loader.loadTexture("Texture_atlas"))
{
  std::cout << "Terrain chunk cache: " << chunkCacheSize << " chunks" << std::endl;
  workerThread = std::thread(&Terrain::worker, this, stopTrigger.get_future());
}

Terrain::~Terrain() {
  std::cout << "shutting down terrain thread..." << std::endl;
  stopTrigger.set_value();
  workerThread.join();
  std::cout << "terrain thread terminated" << std::endl;
}

#include "Debug.hpp"

void Terrain::updateWaitingList() {
  waitingChunks.clear();

  auto insert = [&](ivec2 const& pos) {
    std::lock_guard<std::mutex> lck(chunksMutex);
    for(int i = -renderDistance; i <= renderDistance; i++) {
      ivec3 p = chunkPos + ivec3(pos.x, i, pos.y);
      if(chunks.find(p) == chunks.end()) {
        waitingChunks.push(p);
      }
    }
  };

  insert({0, 0});

  for(int dist = 1; dist < renderDistance+1; dist++) {
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

void Terrain::worker(std::future<void> stopSignal) {
  auto sleep = 1ms;
  auto longSleep = 500ms;
  std::future_status stop;

  do {

    bool changed = ({
      std::lock_guard<std::mutex> lck(chunksMutex);
      bool changed = chunkPosChanged;
      chunkPosChanged = false;
      changed;
    });

    if(changed) {
      TIME(updateWaitingList();)
    }

    if(waitingChunks.size()) {
      ivec3 pos = waitingChunks.pop();
      std::shared_ptr<Chunk> c(generator.generate(pos));
      {
        std::lock_guard<std::mutex> lock(chunksMutex);
        chunks.emplace(pos, c);
        loadedChunks.push(c);
      }
      stop = stopSignal.wait_for(sleep);
    }
    else {
      std::cout << "worker idle" << std::endl;
      stop = stopSignal.wait_for(longSleep);
    }
  } while(stop == std::future_status::timeout);

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
    int dist = distance(it->first, chunkPos);
    if(dist > renderDistance) {
      it = chunks.erase(it);
      count --;
    }
    else
    ++it;
  }

  // unload low-priority chunks
  count = (int)loadedChunks.size();
  for(auto it = loadedChunks.begin(); it != loadedChunks.end() && count > chunkCacheSize;) {
    if (auto chunk = it->lock()) {
      chunk->unload();
    }
    loadedChunks.pop();
    it = loadedChunks.begin();
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
