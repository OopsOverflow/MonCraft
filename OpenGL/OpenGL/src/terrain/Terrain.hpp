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
  void setBlock(glm::ivec3 pos, Block::unique_ptr_t block);

private:
  static const int chunkSize = 32;
  static const int renderDistH = 3; // horizontal render distance (2n+1 chunks)
  static const int renderDistV = 3; // vertical render distance (2n+1 chunks)
  const int chunksMaxCount;

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

  template<class T>
  using Grid3D = std::unordered_map<glm::ivec3, T, ivec3_hash, ivec3_hash>;
  // TODO: should we use runtime alloc instead of compile-time ?
  using WaitingList = AtomicCyclicList<glm::ivec3, (2*renderDistH+1)*(2*renderDistH+1)*(2*renderDistV+1)>;

  Generator generator; // the chunk generator

  glm::vec3 viewDir; // player position
  glm::vec3 playerPos; // player view direction
  glm::ivec3 chunkPos; // in which chunk the player is
  bool chunkPosChanged;
  float fovX;

  // std::array<std::thread, 4> workerThreads; // the worker creates new chunks when it can
  std::thread mainWorkerThread; // manages the queue of chunks to generate
  std::array<std::thread, 4> genWorkerThreads; // creates new chunks when it can
  bool stopFlag;
  std::mutex stopMutex;
  std::condition_variable stopSignal;

  // chunks
  std::mutex chunksMutex;
  Grid3D<std::shared_ptr<Chunk>> chunks; // hashmap to hold the chunks
  WaitingList waitingChunks; // chunk positions yet to be loaded

  Loader loader;
  GLuint texture;

  void mainWorker();
  void genWorker();
  void updateWaitingList();
};
