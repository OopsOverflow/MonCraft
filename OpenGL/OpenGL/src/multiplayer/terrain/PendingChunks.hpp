#pragma once

#include "util/AtomicCyclicList.hpp"
#include "terrain/World.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <thread>

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
