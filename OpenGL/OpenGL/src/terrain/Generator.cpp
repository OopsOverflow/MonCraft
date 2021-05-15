#include "Generator.hpp"
#include "blocks/AllBlocks.hpp"

#include <functional>
#include <map>

using namespace glm;

Generator::Generator(int chunkSize)
  : chunkSize(chunkSize),
    valueNoise(rand())
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

Block::unique_ptr_t Generator::computeSurfaceBlock(ivec3 const& pos, Biome const& biome, int const& blockHeight) const {
    if (abs(pos.y) < 3.f && biome.type == BiomeType::SEA) return Block::create_static<Sand_Block>();
    if (abs(pos.y) < 2.f) return Block::create_static<Sand_Block>();
    if (pos.y <= 0.f) return AllBlocks::create_static(biome.underWaterBlock);
    if (pos.y > 35.f + valueNoise.sample1D(ivec2(pos.x, pos.z) + ivec2(67, 102)) % 5 && biome.surface != BlockType::Ice) return Block::create_static<Snow_Block>();
    if(pos.y > 8.f + valueNoise.sample1D(ivec2(pos.x, pos.z) + ivec2(-12, 0)) % 2 && biome.surface == BlockType::Sand) return Block::create_static<Snow_Block>();
    if (pos.y != blockHeight && biome.surface == BlockType::Grass)return Block::create_static<Dirt_Block>();

    return AllBlocks::create_static(biome.surface);
}

Block::unique_ptr_t Generator::createBlock(ivec3 pos, Biome const& biome) const {
  Block::unique_ptr_t block;
  float height = noise.fractal2(ivec2(pos.x, pos.z), biome.frequencies);
  int blockHeight = (int)floor(height + biome.elevation);

  if(pos.y == blockHeight + 1 && noise.simplex3(pos) * 0.5 + 0.5 < biome.tallgrass && pos.y>0)
    return Block::create_static<Tallgrass_Block>();
  if(pos.y > blockHeight) {
      if (pos.y <= 0) {
          return Block::create_static<Water_Block>();
      }else
      {
          return Block::create_static<Air_Block>();
      }
  }
  if(pos.y >= blockHeight - (int)(valueNoise.sample1D(ivec2(pos.x, pos.z) + ivec2(-91, 859)) % 3))
      return computeSurfaceBlock(pos, biome, blockHeight);

  if(pos.y >= blockHeight - (int)(2+ valueNoise.sample1D(ivec2(pos.x,pos.z) + ivec2(2,-59)) % 3))
    block = AllBlocks::create_static(biome.underLayers);


  return Block::create_static<Stone_Block>();
}

std::vector<Structure::Slice> Generator::generateStructures(ivec3 cpos, Chunk& chunk) const {
  std::vector<Structure::Slice> slices;

  ivec3 dpos(0);
  ivec3 orig = cpos * chunkSize;
  for(dpos.x = 0; dpos.x < chunkSize; dpos.x++) {
    for(dpos.z = 0; dpos.z < chunkSize; dpos.z++) {

      ivec2 posXZ(orig.x + dpos.x, orig.z + dpos.z);

      for(dpos.y = 0; dpos.y < chunkSize; dpos.y++) {

        if(chunk[dpos]->type == BlockType::Grass) {
          ivec3 pos = orig + dpos;
          float tree = valueNoise.sample1D(ivec2(pos.x, pos.z)) / (float)UINT32_MAX;
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
