#pragma once

#include <glm/glm.hpp>
#include <array>

#include "Chunk.hpp"
#include "noise/Noise.hpp"
#include "Structure.hpp"
#include "biomes/BiomeMap.hpp"

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
  Structure::slices_t generateStructures(glm::ivec3 cpos, Chunk& chunk) const;

private:
  Block::unique_ptr_t createBlock(glm::ivec3 pos, Biome const& biome) const;

  int chunkSize;
  SimplexNoise noise;
  SimplexNoise noiseX;
  SimplexNoise noiseY;
  SimplexNoise noiseZ;
  ValueNoise treeNoise;
  Tree defaultTree;

  BiomeMap biomeSampler;
};
