#pragma once

#include <glm/glm.hpp>

#include "blocks/AllBlocks.hpp"
#include "noise/simplex.hpp"
#include "multiplayer/terrain/Structure.hpp"


enum class BiomeType { ICE, SEA, BEACH, TOUNDRA, PLAINS, DESERT, HILLS, MOUNTAINS, FOREST };

struct Biome {
  float elevation; // vertical biome offset
  octaves_t frequencies;
  octaves_t distortion;
  BlockType surface;
  BlockType underLayers;
  BlockType underWaterBlock;
  float trees;
  float tallgrass;
  BiomeType type;
};
