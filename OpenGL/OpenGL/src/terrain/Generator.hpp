#pragma once

#include <glm/glm.hpp>
#include "Chunk.hpp"
#include "noise/Noise.hpp"

class Generator {
public:
  Generator(int chunkSize);

  Chunk* generate(glm::ivec3 pos);

private:
  int chunkSize;
  SimplexNoise noise;
};
