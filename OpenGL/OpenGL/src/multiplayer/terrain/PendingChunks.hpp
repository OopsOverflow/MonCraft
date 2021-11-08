#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <glm/glm.hpp>

class World;

class PendingChunks {

public:
  PendingChunks();
  ~PendingChunks();

  void update(glm::vec3 playerPos);
  bool changed();

  std::vector<glm::ivec3> get();

  void remOldChunks();

private:
  void updateWorker();
  void updateWaitingChunks();

  static const int chunkSize = 16; // TODO: in a config file
  int renderDistH, renderDistV;
  unsigned int maxChunks;
  glm::ivec3 cpos, tcpos;
  bool hasChanged;
  std::vector<glm::ivec3> waitingChunks, newWaitingChunks;
  std::mutex running, copyMutex;
  std::thread thread;
  World& world;
};
