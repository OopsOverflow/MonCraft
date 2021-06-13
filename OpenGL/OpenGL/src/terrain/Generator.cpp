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
    if(pos.y<-5.0f)return Block::create_static<Sandstone_Block>();
    if (abs(pos.y) < 3.f && biome.type == BiomeType::SEA) return Block::create_static<Sand_Block>();
    if (biome.type == BiomeType::MOUNTAINS && abs(abs(pos.y) < 2.f))return Block::create_static<Ice_Block>();
    if (abs(pos.y) < 2.f) return Block::create_static<Sand_Block>();
    if (pos.y <= 0.f) return AllBlocks::create_static(biome.underWaterBlock);
    if (pos.y >= 27.f + valueNoise.sample<2, 1>(ivec2(pos.x, pos.z) + ivec2(72, -402)) % 9) return Block::create_static<Snow_Block>();
    if (pos.y > 25.f + valueNoise.sample<2, 1>(ivec2(pos.x, pos.z) + ivec2(67, 102)) % 5 && biome.surface != BlockType::Ice) return Block::create_static<Stone_Block>();
    if (pos.y > 8.f + valueNoise.sample<2, 1>(ivec2(pos.x, pos.z) + ivec2(-12, 0)) % 2 && biome.surface == BlockType::Sand && biome.type != BiomeType::DESERT) return Block::create_static<Snow_Block>();
    if (pos.y != blockHeight && biome.surface == BlockType::Grass)return Block::create_static<Dirt_Block>();

    return AllBlocks::create_static(biome.surface);
}

Block::unique_ptr_t Generator::createBlock(ivec3 pos, Biome const& biome) const {
  float height = noise.fractal2(ivec2(pos.x, pos.z), biome.frequencies);
  int blockHeight = (int)floor(height + biome.elevation);

  //blockHeight = (int)floor(biome.elevation);

  if(pos.y>0 && pos.y<27 && pos.y == blockHeight + 1 && noise.simplex3(pos) * 0.5 + 0.5 < biome.tallgrass && pos.y> valueNoise.sample<2, 1>(ivec2(pos.x, pos.z) + ivec2(3, -7)) % 4)
    return Block::create_static<Tallgrass_Block>();
  if(pos.y > blockHeight) {
      if (pos.y <= 0) {
          return Block::create_static<Water_Block>();
      }else
      {
          return Block::create_static<Air_Block>();
      }
  }
  if(pos.y >= blockHeight - (int)(valueNoise.sample<2, 1>(ivec2(pos.x, pos.z) + ivec2(-91, 859)) % 3))
      return computeSurfaceBlock(pos, biome, blockHeight);
  // COMBAK: are we not using this anymore ? That piece of code was useless
  // if(pos.y >= blockHeight - (int)(2+ valueNoise.sample1D(ivec2(pos.x,pos.z) + ivec2(2,-59)) % 3))
  //   block = AllBlocks::create_static(biome.underLayers);

  //if you want cave update :)
  if(pos.y>= blockHeight -5 - valueNoise.sample<2, 1>(ivec2(pos.x,pos.z) + ivec2(158,-804)) % 5) return Block::create_static<Stone_Block>();
  float caveNoise = noise.perlin3((vec3)(pos + ivec3(55, 8, -95)) * 0.08f);
  if(caveNoise >0.4 && caveNoise<0.65)return Block::create_static<Air_Block>();
  if (noise.perlin3((vec3)(pos + ivec3(-614, 120, 745)) * 0.1f) > 0.3)return Block::create_static<Sandstone_Block>();
  return Block::create_static<Stone_Block>();
}

std::vector<Structure::Slice> Generator::generateStructures(Chunk& chunk) const {
  std::vector<Structure::Slice> slices;
  ivec3 dpos(0);
  ivec3 orig = chunk.chunkPos * chunkSize;
  for(dpos.x = 0; dpos.x < chunkSize; dpos.x++) {
    for(dpos.z = 0; dpos.z < chunkSize; dpos.z++) {

      ivec2 posXZ(orig.x + dpos.x, orig.z + dpos.z);
      auto const& biome = biomeSampler.sampleWeighted(posXZ);
      for(dpos.y = 0; dpos.y < chunkSize; dpos.y++) {
        ivec3 pos = orig + dpos;

        if(chunk[dpos]->type == BlockType::Grass) {
          float amount = 0.005f;
          if (biome.type == BiomeType::FOREST)amount = 0.02f;
          uint32 tree = valueNoise.sample<2, 1>(posXZ);
          if(tree / (float)UINT32_MAX < amount) {
            auto treeSlices = (tree%2?defaultOakTree.spawn(chunk, dpos): defaultBirchTree.spawn(chunk, dpos));
            slices.insert(slices.end(), treeSlices.begin(), treeSlices.end());
          }
        }
        if (chunk[dpos]->type == BlockType::Sand && biome.type==BiomeType::DESERT) {
            float cactus = valueNoise.sample<2, 1>(posXZ) / (float)UINT32_MAX;
            if (cactus < 0.0010f && pos.y>0) {
                auto cactusSlice = defaultCactus.spawn(chunk, dpos);
                slices.insert(slices.end(), cactusSlice.begin(), cactusSlice.end());
            }
        }

        if (chunk[dpos]->type == BlockType::Snow && biome.type == BiomeType::TOUNDRA) {
            float editPlateforme = valueNoise.sample<2, 1>(posXZ) / (float)UINT32_MAX;
            if (editPlateforme < 0.0000f) {
                auto editPlateformeSlice = defaultEditPlaterforme.spawn(chunk, dpos);
                slices.insert(slices.end(), editPlateformeSlice.begin(), editPlateformeSlice.end());
            }
        }

      }
    }
  }

  return slices;
}
