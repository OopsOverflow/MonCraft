#pragma once

#include "util/AtomicCyclicList.hpp"
#include "terrain/World.hpp"

#include <glm/glm.hpp>
#include <vector>

class PendingChunks {

public:
  PendingChunks();

  bool changed(glm::vec3 playerPos);

  std::vector<glm::ivec3> get();

  void remOldChunks();

private:
  void updateWaitingChunks();

  static const int chunkSize = 16; // TODO: in a config file
  int renderDistH, renderDistV;
  unsigned int maxChunks;
  glm::ivec3 cpos;
  std::vector<glm::ivec3> waitingChunks;
  World& world;
};
