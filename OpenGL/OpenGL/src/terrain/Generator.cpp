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

Chunk* Generator::generate(ivec3 pos) {

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
      ivec2 pos2D = ivec2(pos.x, pos.z) * ivec2(chunkSize) + dpos;
      float height = noise.fractal2(pos2D, octaves) * .5 + .5;
      int blockHeight = (int)floor(height * chunkSize) - pos.y * chunkSize;

      if(blockHeight < -1) {
        for(int i = -1; i < chunkSize + 1; i++) {
          blocks[ivec3(dpos.x, i, dpos.y) + one] = std::make_unique<Air_Block>();
        }
      }

      else if(blockHeight > chunkSize) {
        for(int i = -1; i < chunkSize + 1; i++) {
          blocks[ivec3(dpos.x, i, dpos.y) + one] = std::make_unique<Dirt_Block>();
        }
      }

      else {
        // 1) air column
        for(int i = chunkSize; i > blockHeight; i--) {
          blocks[ivec3(dpos.x, i, dpos.y) + one] = std::make_unique<Air_Block>();
        }

        // 2) single grass block
        blocks[ivec3(dpos.x, blockHeight, dpos.y) + one] = std::make_unique<Grass_Block>();

        // 3) dirt column
        for(int i = blockHeight-1; i >= -1; i--) {
          blocks[ivec3(dpos.x, i, dpos.y) + one] = std::make_unique<Dirt_Block>();
        }
      }
    }
  }
  return new Chunk(pos * ivec3(chunkSize), std::move(blocks));
}
