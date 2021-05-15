#pragma once

#include <glm/glm.hpp>
#include "blocks/AllBlocks.hpp"
#include "noise/Noise.hpp"
#include "terrain/Structure.hpp"


enum class BiomeType { ICE, SEA, BEACH, TOUNDRA, PLAINS, DESERT, HILLS, MOUNTAINS };

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


  BlockType generate(); // first pass:
  void decorate(); // ran after generation to add decorations and such
  Structure::slices_t generateStructures(); // spawn structures
};
