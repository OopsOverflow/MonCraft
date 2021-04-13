#pragma once

#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Chunk.hpp"
#include "Generator.hpp"
#include "Camera.hpp"

class Terrain
{
public:
  Terrain();
  ~Terrain();

  Terrain(Terrain const&) = delete;
  Terrain& operator=(Terrain const&) = delete;

  void render(Camera const& cam);

  const int chunkSize = 16;
  const int renderDistance = 8;
  const int chunksMaxCount = (int)pow(renderDistance * 2 + 1, 2);

private:
  // dirty hash function for the chunks hashmap
  struct ivec2_hash
  {
    size_t operator()(glm::ivec2 const& k) const {
      return std::hash<int>()(k.x << 16 | k.y >> 16);
    }
    bool operator()(glm::ivec2 const& a, glm::ivec2 const& b) const {
      return a.x == b.x && a.y == b.y;
    }
  };
  using ChunkMap = std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, ivec2_hash, ivec2_hash>;

  Generator generator; // the chunk generator
  glm::ivec2 chunkPos; // in which chunk the player is

  std::thread workerThread; // the worker creates new chunks when it can
  std::promise<void> stopTrigger; // asks the thread to terminate

  std::mutex chunksMutex;
  ChunkMap chunks;

   // this is where the worker puts the generated chunks that are later
   // moved to the hashmap by the main thread.
  std::mutex dataMutex;
  std::vector<std::pair<glm::ivec2, Chunk*>> data;

  void worker(std::future<void> stopSignal);
  void update();

  // this is kinda dirty, see cpp file.
  glm::ivec2 lastPos = glm::ivec2(0);
  int last_i = 0, last_j = -1;
  bool getNextPos(glm::ivec2 &pos);
};
