#pragma once

#include <glm/glm.hpp>
#include "Chunk.hpp"
#include "../noise/Noise.hpp"
#include "Structure.hpp"

/**
* Generates new chunks.
*/

class Generator {
public:
  Generator(int chunkSize);

  /**
  * Generates a chunk at the given chunk index.
  */
  std::unique_ptr<Chunk> generate(glm::ivec3 cpos) const;
  std::vector<Structure::Slice> generateStructures(glm::ivec3 cpos, Chunk& chunk) const;

private:
  int chunkSize;
  SimplexNoise noise;
  SimplexNoise noiseX;
  SimplexNoise noiseY;
  SimplexNoise noiseZ;
  ValueNoise treeNoise;
  Tree defaultTree;
};
