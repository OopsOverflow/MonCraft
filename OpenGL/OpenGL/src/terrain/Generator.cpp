#include "Generator.hpp"
#include "../blocks/Grass_Block.h"
#include "../blocks/Dirt_Block.h"
#include "../blocks/Air_Block.h"
#include "../blocks/Debug_Block.h"

using glm::ivec2;
using glm::ivec3;

Generator::Generator(int chunkSize)
  : chunkSize(chunkSize)
{
  noise.seed(27);
}

Chunk* Generator::generate(ivec2 pos) {

  static const octaves_t octaves = {
    {.65f, 0.004f}, // {magnitude, frequency}
    {.20f, 0.008f},
    {.10f, 0.050f},
    {.05f, 0.100f},
  };

  Blocks blocks(chunkSize + 2);

  const ivec3 one(1);

  ivec2 dpos(0);
  for(dpos.x = -1; dpos.x <= chunkSize; dpos.x++) {
    for(dpos.y = -1; dpos.y <= chunkSize; dpos.y++) {
      ivec2 pos2D = pos * ivec2(chunkSize) + dpos;
      float height = (noise.fractal2(pos2D, octaves) + 1) / 2;
      int blockHeight = (int)floor(height * chunkSize);

      // 1) air column
      for(int i = chunkSize; i > blockHeight; i--) {
        blocks.at(ivec3(dpos.x, i, dpos.y) + one) = std::make_unique<Air_Block>();
      }

      // 1) single grass block
      blocks.at(ivec3(dpos.x, blockHeight, dpos.y) + one) = std::make_unique<Grass_Block>();

      // 3) dirt column
      for(int i = blockHeight-1; i >= 0; i--) {
        blocks.at(ivec3(dpos.x, i, dpos.y) + one) = std::make_unique<Dirt_Block>();
      }

      // 3) below chunk = void
      blocks.at(ivec3(dpos.x, 0, dpos.y) + one) = std::make_unique<Air_Block>();
    }
  }
  return new Chunk(pos * ivec2(chunkSize), std::move(blocks)); // TODO
}
