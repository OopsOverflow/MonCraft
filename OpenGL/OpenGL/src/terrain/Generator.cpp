#include "Generator.hpp"

using glm::ivec2;
using glm::ivec3;

Generator::Generator(int chunkSize)
  : chunkSize(chunkSize)
{
  noise.seed(27);
}

Chunk* Generator::generate(ivec2 pos) {

  static const octaves_t octaves = {
    {.65, 0.004}, // {magnitude, frequency}
    {.20, 0.008},
    {.10, 0.050},
    {.05, 0.100},
  };

  Blocks blocks(chunkSize + 2);

  const ivec3 one(1);

  ivec2 dpos;
  for(dpos.x = -1; dpos.x <= chunkSize; dpos.x++) {
    for(dpos.y = -1; dpos.y <= chunkSize; dpos.y++) {
      ivec2 pos2D = pos * ivec2(chunkSize) + dpos;
      float height = (noise.fractal2(pos2D, octaves) + 1) / 2;
      int blockHeight = (int)floor(height * chunkSize);

      // 1) air column
      for(int i = chunkSize; i > blockHeight; i--) {
        blocks.at(ivec3(dpos.x, i, dpos.y) + one) = BlockType::Air;
      }

      // 1) single grass block
      blocks.at(ivec3(dpos.x, blockHeight, dpos.y) + one) = BlockType::Grass;

      // 3) dirt column
      for(int i = blockHeight-1; i >= 0; i--) {
        blocks.at(ivec3(dpos.x, i, dpos.y) + one) = BlockType::Dirt;
      }

      // 3) below chunk = void
      blocks.at(ivec3(dpos.x, 0, dpos.y) + one) = BlockType::Air;

    }
  }
  return new Chunk(pos * ivec2(chunkSize), std::move(blocks)); // TODO
}
