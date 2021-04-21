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

  const int chunkSize = 32;
  const int renderDistance = 3;
  const int chunksMaxCount = (int)pow(renderDistance * 2 + 1, 3);

private:
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
  using ChunkMap   = std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>, ivec3_hash, ivec3_hash>;

  Generator generator; // the chunk generator

  glm::vec3 viewDir; // player position
  glm::vec3 playerPos; // player view direction
  glm::ivec3 chunkPos; // in which chunk the player is
  float fovX;

  std::thread workerThread; // the worker creates new chunks when it can
  std::promise<void> stopTrigger; // asks the thread to terminate

  std::mutex chunksMutex;
  ChunkMap chunks;

  void worker(std::future<void> stopSignal);

  Loader loader;
  GLuint texture;

  // this is kinda dirty, see cpp file.
  glm::ivec3 lastPos = glm::ivec3(0);
  glm::ivec3 lastIter = glm::ivec3(0, -renderDistance, -1);
  bool getNextPos(glm::ivec3 &pos);
};
