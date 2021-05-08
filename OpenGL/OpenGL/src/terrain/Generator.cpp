#include "Generator.hpp"
#include "blocks/Grass_Block.hpp"
#include "blocks/Dirt_Block.hpp"
#include "blocks/Air_Block.hpp"
#include "blocks/Stone_Block.hpp"
#include "blocks/Debug_Block.hpp"
#include "blocks/Leaf_Block.hpp"

using namespace glm;

Generator::Generator(int chunkSize)
  : chunkSize(chunkSize),
    treeNoise(92847)
{
  noise.seed(27);
  noiseX.seed(1156);
  noiseY.seed(9848);
  noiseZ.seed(21554);
}

std::unique_ptr<Chunk> Generator::generate(ivec3 cpos) const {

  static const int maxHeight = 50;

  static const octaves_t octaves = {
    {.60f, 0.004f}, // {magnitude, frequency}
    {.35f, 0.008f},
    {.05f, 0.050f},
    {.00f, 0.100f},
  };

  std::unique_ptr<Chunk> chunk(new Chunk(cpos, chunkSize));

  ivec3 dpos(0);
  for(dpos.x = 0; dpos.x < chunkSize; dpos.x++) {
    for(dpos.y = 0; dpos.y < chunkSize; dpos.y++) {
      for(dpos.z = 0; dpos.z < chunkSize; dpos.z++) {

        ivec3 pos = cpos * chunkSize + dpos;

        float height = noise.fractal2(ivec2(pos.x, pos.z), octaves) * .5f + .5f;
        int blockHeight = (int)floor(height * maxHeight) - cpos.y * chunkSize;
        auto& block = (*chunk)[dpos];
        float tree = treeNoise.sample1D(ivec2(pos.x, pos.z)) / (float)UINT32_MAX;

        if(dpos.y == blockHeight + 1 && tree < 0.005) {
          block = Block::create_static<Leaf_Block>();
        }
        else if(dpos.y > blockHeight) {
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

std::vector<Structure::Slice> Generator::generateStructures(ivec3 cpos, Chunk& chunk) const {
  std::vector<Structure::Slice> slices;

  ivec3 dpos(0);
  for(dpos.x = 0; dpos.x < chunkSize; dpos.x++) {
    for(dpos.y = 0; dpos.y < chunkSize; dpos.y++) {
      for(dpos.z = 0; dpos.z < chunkSize; dpos.z++) {

        if(chunk[dpos]->type == BlockType::Grass) {
          ivec3 pos = cpos * chunkSize + dpos;
          float tree = treeNoise.sample1D(ivec2(pos.x, pos.z)) / (float)UINT32_MAX;
          if(tree < 0.005) {
            auto treeSlices = defaultTree.spawn(chunk, dpos);
            slices.insert(slices.end(), treeSlices.begin(), treeSlices.end());
          }
        }

      }
    }
  }

  return slices;
}
