#pragma once

#include <glm/glm.hpp>
#include "Chunk.hpp"
#include "../noise/Noise.hpp"

class Generator {
public:
  Generator(int chunkSize);

  std::shared_ptr<Chunk> generate(glm::ivec3 pos);

private:
  int chunkSize;
  SimplexNoise noise;
  SimplexNoise noiseX;
  SimplexNoise noiseY;
  SimplexNoise noiseZ;
};
