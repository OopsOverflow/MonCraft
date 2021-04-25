#include "Generator.hpp"
#include "../blocks/Grass_Block.h"
#include "../blocks/Dirt_Block.h"
#include "../blocks/Air_Block.h"
#include "../blocks/Debug_Block.h"
#include "../blocks/Stone_Block.h"

using glm::ivec2;
using glm::ivec3;
using glm::vec3;

Generator::Generator(int chunkSize)
  : chunkSize(chunkSize)
{
  noise.seed(27);
  noiseX.seed(1156);
  noiseY.seed(9848);
  noiseZ.seed(21554);
}

Chunk* Generator::generate(ivec3 chunkPos) {

  static const int maxHeight = 100;

  static const float distortFreq = 0.0001f;
  static const float distortAmpl = 1000.f;

  static const octaves_t octaves = {
    {.65f, 0.004f}, // {magnitude, frequency}
    {.35f, 0.008f},
    {.00f, 0.050f},
    {.00f, 0.100f},
  };

  Blocks blocks(chunkSize + 2);

  const ivec3 one(1);

  ivec3 dpos(0);

  for(dpos.x = -1; dpos.x <= chunkSize; dpos.x++) {
    for(dpos.y = -1; dpos.y <= chunkSize; dpos.y++) {
      for(dpos.z = -1; dpos.z <= chunkSize; dpos.z++) {

        ivec3 pos = chunkPos * chunkSize + dpos;

        vec3 distort = vec3(
          noiseX.simplex3(vec3(pos) * distortFreq),
          noiseY.simplex3(vec3(pos) * distortFreq),
          noiseZ.simplex3(vec3(pos) * distortFreq)
        ) * distortAmpl;

        distort = abs(distort) - distortAmpl / 2.f;
        distort *= 2.f;

        pos += distort;

        float height = noise.fractal2(ivec2(pos.x, pos.z), octaves) * .5f + .5f;
        int blockHeight = (int)floor(height * maxHeight) - chunkPos.y * chunkSize;
        auto& block = blocks[dpos + one];

        if(dpos.y > blockHeight) {
          block = Blocks::create_static<Air_Block>();
        }

        else if(dpos.y == blockHeight) {
          block = Blocks::create_static<Grass_Block>();
        }
        else if(dpos.y >= blockHeight - 3) {
          block = Blocks::create_static<Dirt_Block>();
        }
        else {
          block = Blocks::create_static<Stone_Block>();
        }

      }
    }
  }

  return new Chunk(chunkPos * ivec3(chunkSize), std::move(blocks));
}
