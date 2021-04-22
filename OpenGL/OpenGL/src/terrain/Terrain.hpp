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
#include "../util/Loader.hpp"
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
  static const int renderDistance = 8;
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
  using WaitingList = AtomicCyclicList<glm::ivec3, (2*renderDistance+1)*(2*renderDistance+1)*(2*renderDistance+1)>;

  Generator generator; // the chunk generator

  glm::vec3 viewDir; // player position
  glm::vec3 playerPos; // player view direction
  glm::ivec3 chunkPos; // in which chunk the player is
  bool chunkPosChanged;
  float fovX;

  std::thread workerThread; // the worker creates new chunks when it can
  std::promise<void> stopTrigger; // asks the worker thread to terminate

  // chunks
  std::mutex chunksMutex;
  ChunkMap chunks; // hashmap to hold the chunks
  ChunkPList loadedChunks; // cache of fully loaded chunks
  WaitingList waitingChunks; // chunk positions yet to be loaded

  Loader loader;
  GLuint texture;

  void worker(std::future<void> stopSignal);

  // this is kinda dirty, see cpp file.
  // glm::ivec3 lastIter = glm::ivec3(0, -renderDistance, -1);
  // glm::ivec3 lastPos = glm::ivec3(0);
  void updateWaitingList();
};
