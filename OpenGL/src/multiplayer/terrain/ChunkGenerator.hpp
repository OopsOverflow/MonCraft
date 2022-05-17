#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Structure.hpp"
#include "BiomeMap.hpp"
#include "blocks/Block.hpp"
#include "noise/XXHash.hpp"
#include "noise/simplex.hpp"
#include "terrain/ChunkImpl.hpp"

struct Biome;

/**
* Generates new chunks.
*/

class ChunkGenerator {
public:
  ChunkGenerator(int chunkSize);

  /**
  * Generates a chunk at the given chunk index.
  */
  std::unique_ptr<ChunkImpl> generate(glm::ivec3 cpos) const;

  /**
  * Generates the structures inside the given chunk.
  * returns a list of slices to be applied to the chunks.
  */
  Structure::slices_t generateStructures(Chunk& chunk) const;

private:
  Block::unique_ptr_t createBlock(glm::ivec3 pos, Biome const& biome) const;

  Block::unique_ptr_t computeSurfaceBlock(glm::ivec3 const& pos, Biome const& biome, int const& blockHeight) const;

  int chunkSize;
  SimplexNoise noise;
  SimplexNoise noiseX;
  SimplexNoise noiseY;
  SimplexNoise noiseZ;
  XXHash valueNoise;
  Oak_Tree defaultOakTree;
  Birch_Tree defaultBirchTree;
  Cactus defaultCactus;
  EditPlateforme defaultEditPlaterforme;
public:
  BiomeMap biomeSampler;
};
