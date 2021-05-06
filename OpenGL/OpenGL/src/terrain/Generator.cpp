#include "Generator.hpp"
#include "../blocks/Grass_Block.h"
#include "../blocks/Dirt_Block.h"
#include "../blocks/Air_Block.h"
#include "../blocks/Stone_Block.h"
#include "../blocks/Debug_Block.h"

using namespace glm;

Generator::Generator(int chunkSize)
  : chunkSize(chunkSize)
{
  noise.seed(27);
  noiseX.seed(1156);
  noiseY.seed(9848);
  noiseZ.seed(21554);
}

std::shared_ptr<Chunk> Generator::generate(ivec3 chunkPos) {

  static const int maxHeight = 100;

  static const octaves_t octaves = {
    {.60f, 0.004f}, // {magnitude, frequency}
    {.35f, 0.008f},
    {.05f, 0.050f},
    {.00f, 0.100f},
  };

  std::shared_ptr<Chunk> chunk(new Chunk(chunkPos, chunkSize));

  ivec3 dpos(0);

  for(dpos.x = 0; dpos.x < chunkSize; dpos.x++) {
    for(dpos.y = 0; dpos.y < chunkSize; dpos.y++) {
      for(dpos.z = 0; dpos.z < chunkSize; dpos.z++) {

        ivec3 pos = chunkPos * chunkSize + dpos;

        float height = noise.fractal2(ivec2(pos.x, pos.z), octaves) * .5f + .5f;
        int blockHeight = (int)floor(height * maxHeight) - chunkPos.y * chunkSize;
        auto& block = (*chunk)[dpos];

        if(dpos.y > blockHeight) {
          block = Block::create_static<Air_Block>();
        }
        else if(dpos.y == blockHeight) {
          block = Block::create_static<Grass_Block>();
        }
        else if(dpos.y >= blockHeight - 3) {
          block = Block::create_static<Dirt_Block>();
        }
        else {
          block = Block::create_static<Stone_Block>();
        }

      }
    }
  }

  return chunk;
}
