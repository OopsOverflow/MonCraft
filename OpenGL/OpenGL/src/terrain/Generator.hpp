#pragma once

#include <glm/glm.hpp>
#include "Chunk.hpp"
#include "Noise.hpp"

class Generator {
public:
  Generator(int chunkSize);

  Chunk* generate(glm::ivec2 pos);

private:
  int chunkSize;
  SimplexNoise noise;
};
