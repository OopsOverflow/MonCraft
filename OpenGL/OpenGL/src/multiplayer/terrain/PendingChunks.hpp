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
  static const int renderDistH = 5; // horizontal render distance (2n+1 chunks)
  static const int renderDistV = 5; // vertical render distance (2n+1 chunks)
  static const int maxChunks = renderDistH * renderDistH * renderDistV;
  glm::ivec3 cpos;
  std::vector<glm::ivec3> waitingChunks;
  World& world;
};
