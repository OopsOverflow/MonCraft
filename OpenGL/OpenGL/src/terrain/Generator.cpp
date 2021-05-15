#include "Generator.hpp"
#include "blocks/AllBlocks.hpp"

#include <functional>
#include <map>

using namespace glm;

Generator::Generator(int chunkSize)
  : chunkSize(chunkSize),
    treeNoise(rand())
{
  noise.seed(rand());
  biomeSampler.generate();
}

std::unique_ptr<Chunk> Generator::generate(ivec3 cpos) const {

  std::unique_ptr<Chunk> chunk(new Chunk(cpos, chunkSize));

  ivec3 dpos(0);
  ivec3 orig = cpos * chunkSize;
  for(dpos.x = 0; dpos.x < chunkSize; dpos.x++) {
    for(dpos.z = 0; dpos.z < chunkSize; dpos.z++) {

      ivec2 posXZ(orig.x + dpos.x, orig.z + dpos.z);
      auto const& biome = biomeSampler.sampleWeighted(posXZ);

      for(dpos.y = 0; dpos.y < chunkSize; dpos.y++) {
        ivec3 pos = orig + dpos;
        (*chunk)[dpos] = createBlock(pos, biome);
      }
    }
  }

  return chunk;
}

Block::unique_ptr_t Generator::createBlock(ivec3 pos, Biome const& biome) const {
  Block::unique_ptr_t block;
  float height = noise.fractal2(ivec2(pos.x, pos.z), biome.frequencies);
  int blockHeight = (int)floor(height + biome.elevation);

  if(pos.y == blockHeight + 1 && noise.simplex3(pos) * 0.5 + 0.5 < biome.tallgrass) {
    block = Block::create_static<Tallgrass_Block>();
  }
  else if(pos.y > blockHeight) {
      if (pos.y < 0.0f) {
          block = Block::create_static<Water_Block>();
      }else
      {
          block = Block::create_static<Air_Block>();
      }

  }
  else if(pos.y == blockHeight) {
    block = AllBlocks::create_static(biome.surface);
  }
  else if(pos.y >= blockHeight - 3) {
    block = AllBlocks::create_static(biome.underLayers);
  }
  else {
    block = Block::create_static<Stone_Block>();
  }

  return block;
}

std::vector<Structure::Slice> Generator::generateStructures(ivec3 cpos, Chunk& chunk) const {
  std::vector<Structure::Slice> slices;

  ivec3 dpos(0);
  ivec3 orig = cpos * chunkSize;
  for(dpos.x = 0; dpos.x < chunkSize; dpos.x++) {
    for(dpos.z = 0; dpos.z < chunkSize; dpos.z++) {

      ivec2 posXZ(orig.x + dpos.x, orig.z + dpos.z);
      // auto biome = biomeSampler.at(posXZ); // TODO

      for(dpos.y = 0; dpos.y < chunkSize; dpos.y++) {

        if(chunk[dpos]->type == BlockType::Grass) {
          ivec3 pos = orig + dpos;
          float tree = treeNoise.sample1D(ivec2(pos.x, pos.z)) / (float)UINT32_MAX;
          if(tree < 0.0005f) {
            auto treeSlices = defaultTree.spawn(chunk, dpos);
            slices.insert(slices.end(), treeSlices.begin(), treeSlices.end());
          }
        }

      }
    }
  }

  return slices;
}
