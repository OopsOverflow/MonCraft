#pragma once

#include <glm/glm.hpp>
#include "blocks/AllBlocks.hpp"
#include "noise/Noise.hpp"
#include "terrain/Structure.hpp"


struct Biome {
  float elevation; // vertical biome offset
  octaves_t frequencies;
  octaves_t distortion;
  BlockType surface;
  BlockType underLayers;
  float trees;
  float tallgrass;

  BlockType generate(); // first pass:
  void decorate(); // ran after generation to add decorations and such
  Structure::slices_t generateStructures(); // spawn structures
};
