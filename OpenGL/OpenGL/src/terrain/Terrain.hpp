#pragma once

#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Chunk.hpp"
#include "Generator.hpp"
#include "../gl/Camera.hpp"
#include "../gl/Loader.hpp"
#include "../util/PriorityList.hpp"
#include "../util/AtomicCyclicList.hpp"

class Terrain
{
public:
  Terrain();
  ~Terrain();

  Terrain(Terrain const&) = delete;
  Terrain& operator=(Terrain const&) = delete;

  void update(glm::vec3 pos, glm::vec3 dir, float fovX);
  void render();

  Block* getBlock(glm::ivec3 pos);

private:
  static const int chunkSize = 32;
  static const int renderDistH = 8; // horizontal render distance (2n+1 chunks)
  static const int renderDistV = 3; // vertical render distance (2n+1 chunks)
  const int chunksMaxCount;

  const int memoryCap = 2048; // max amount of memory (mebibytes)
  const int chunkMemorySize;  // size of a single chunk in memory (kibibytes)
  const int chunkCacheSize;   // maximum number of chunks in cache


  // dirty hash function for the chunks hashmap
  struct ivec3_hash
  {
    size_t operator()(glm::ivec3 const& k) const {
      return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^  std::hash<int>()(k.z);
    }
    bool operator()(glm::ivec3 const& a, glm::ivec3 const& b) const {
      return a == b;
    }
  };


  using ChunkMap = std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>, ivec3_hash, ivec3_hash>;
  using ChunkPList = PriorityList<std::weak_ptr<Chunk>>;
  // TODO: should we use runtime alloc instead of compile-time ?
  using WaitingList = AtomicCyclicList<glm::ivec3, (2*renderDistH+1)*(2*renderDistH+1)*(2*renderDistV+1)>;

  Generator generator; // the chunk generator

  glm::vec3 viewDir; // player position
  glm::vec3 playerPos; // player view direction
  glm::ivec3 chunkPos; // in which chunk the player is
  bool chunkPosChanged;
  float fovX;

  std::array<std::thread, 4> workerThreads; // the worker creates new chunks when it can
  bool stopFlag;
  std::mutex stopMutex;
  std::condition_variable stopSignal;

  // chunks
  std::mutex chunksMutex;
  ChunkMap chunks; // hashmap to hold the chunks
  ChunkPList loadedChunks; // cache of fully loaded chunks
  WaitingList waitingChunks; // chunk positions yet to be loaded

  Loader loader;
  GLuint texture;

  void worker(int n);

  // this is kinda dirty, see cpp file.
  // glm::ivec3 lastIter = glm::ivec3(0, -renderDistance, -1);
  // glm::ivec3 lastPos = glm::ivec3(0);
  void updateWaitingList();
};
