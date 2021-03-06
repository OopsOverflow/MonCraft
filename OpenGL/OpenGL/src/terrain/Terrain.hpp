#pragma once

#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <unordered_map>
#include <glm/glm.hpp>

#include "ChunkMap.hpp"
#include "SliceMap.hpp"
#include "Generator.hpp"
#include "gl/Camera.hpp"
#include "util/PriorityList.hpp"
#include "util/AtomicCyclicList.hpp"
#include "save/SaveManager.hpp"

/**
* Manages the chunks, load/unload, render and manipulation of blocks.
*/

class Terrain
{
public:
  Terrain();
  ~Terrain();

  Terrain(Terrain const&) = delete;
  Terrain& operator=(Terrain const&) = delete;

  /**
  *
  */
  void update(glm::vec3 pos);

  /**
  * Renders the visible chunks.
  */
  void render(Camera const& camera);

  /**
  * Gets a block in the world.
  * if block is unavailable, returns nullptr.
  */
  Block* getBlock(glm::ivec3 pos);

  /**
  * Sets a block in the world.
  * throws if the block is unavailable.
  */
  void setBlock(glm::ivec3 pos, Block::unique_ptr_t block);

  void startGeneration();
  void stopGeneration();
  void toggleGeneration();
  bool generating;

private:
  static const int chunkSize = 16;
  static const int renderDistH = 10; // horizontal render distance (2n+1 chunks)
  static const int renderDistV = 5; // vertical render distance (2n+1 chunks)
  const int chunksMaxCount;

  // dirty hash function for the chunks hashmap
  // (this may need to be improved ?)
  struct ivec3_hash
  {
    size_t operator()(glm::ivec3 const& k) const {
      return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^  std::hash<int>()(k.z);
    }
    bool operator()(glm::ivec3 const& a, glm::ivec3 const& b) const {
      return a == b;
    }
  };

  // TODO: should we use runtime alloc instead of compile-time ? Post release
  using WaitingList = AtomicCyclicList<glm::ivec3, (2*renderDistH+1)*(2*renderDistH+1)*(2*renderDistV+1)>;

  Generator generator; // the chunk generator

  glm::ivec3 chunkPos; // in which chunk the player is
  bool chunkPosChanged;

  // threading
  std::thread mainWorkerThread; // manages the queue of chunks to generate
  static const int N_THREADS = 4;
  std::array<std::thread, N_THREADS> genWorkerThreads; // creates new chunks when it can
  std::mutex workerMutex;
  void mainWorker();
  void genWorker();

  // utilities for workers
  bool sleepFor(std::chrono::milliseconds);
  void updateWaitingList();

  std::vector<glm::ivec3> busyList; // list of chunks beiing generated
  std::mutex busyListMutex;
  bool addInBusyList(glm::ivec3 cpos);
  void remFromBusyList(glm::ivec3 cpos);

  std::mutex posMutex;
  glm::ivec3 getChunkPos();
  void setChunkPos(glm::ivec3 cpos);
  bool hasPosChanged();

  // signals to stop the threads
  bool stopFlag;
  std::mutex stopMutex;
  std::condition_variable stopSignal;

  // chunk storage
  ChunkMap chunkMap; // hashmap to hold the chunks
  SliceMap sliceMap; // hashmap to hold the slices generated by structures
  WaitingList waitingChunks; // chunk positions yet to be loaded
  SaveManager save;
};
