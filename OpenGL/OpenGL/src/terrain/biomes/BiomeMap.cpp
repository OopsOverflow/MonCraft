#include "BiomeMap.hpp"
#include <functional>
#include <algorithm>

using namespace glm;
using namespace std::placeholders;

// spatial distortion octaves
const octaves_t octaves = {
  {.65f, 0.004f}, // {magnitude, frequency}
  {.20f, 0.008f},
  {.10f, 0.050f},
  {.05f, 0.100f},
};

// temperature / dryness octaves
const octaves_t tempOctaves = {
  {1, .08f},
};

// altitude octaves
const octaves_t heightOctaves = {
  {1, 1.f},
};

// moisture/water level octaves
const octaves_t waterOctaves = {
  {1, 0.1f},
};

BiomeMap::BiomeMap()
 : voronoi(rand(), gridSize),
   value(rand()),
   grid(size + 2 * displacement),
   map(size)
{
  simplexX.seed(rand());
  simplexY.seed(rand());
  simplexBiome.seed(rand());

  biomePlains.elevation = 0;
  biomePlains.distortion = {};
  biomePlains.frequencies = {
    {6.0f, 0.004f}, // {magnitude, frequency}
    {4.5f, 0.008f},
    {1.0f, 0.050f},
    {0.5f, 0.100f},
  };
  biomePlains.surface = BlockType::Grass;
  biomePlains.underLayers = BlockType::Dirt;
  biomePlains.trees = 0.05f;
  biomePlains.tallgrass = 0.2f;

  biomeDesert = biomePlains;
  biomeDesert.surface = BlockType::Sand;
  biomeDesert.underLayers = BlockType::Sand;
  biomeDesert.frequencies = {
    {6.0f, 0.004f}, // {magnitude, frequency}
    {3.5f, 0.008f},
    {0.0f, 0.050f},
    {0.0f, 0.100f},
  };
  biomeDesert.elevation = 5;
  biomeDesert.tallgrass = 0;

  biomeSea = biomePlains;
  biomeSea.surface = BlockType::Sandstone;
  biomeSea.underLayers = BlockType::Sandstone;
  biomeSea.elevation = -20;
  biomeSea.tallgrass = 0;

  biomeToundra = biomePlains;
  biomeToundra.surface = BlockType::Snow;
  biomeToundra.underLayers = BlockType::Dirt;
  biomeToundra.tallgrass = 0;
  biomeToundra.elevation = 5;

  biomeHills = biomePlains;
  biomeHills.surface = BlockType::Grass;
  biomeHills.underLayers = BlockType::Dirt;
  biomeDesert.frequencies = {
    {6.0f, 0.004f}, // {magnitude, frequency}
    {3.5f, 0.008f},
    {0.0f, 0.050f},
    {0.0f, 0.100f},
  };
  // biomeHills.tallgrass = 0;
  biomeHills.elevation = 10;

  biomeMountains = biomePlains;
  biomeMountains.surface = BlockType::Ice;
  biomeMountains.underLayers = BlockType::Stone;
  biomeMountains.frequencies = {
    {0.0f, 0.004f}, // {magnitude, frequency}
    {30.0f, 0.006f},
    {2.0f, 0.050f},
    {1.5f, 0.100f},
  };
  biomeMountains.elevation = 15;
}


void BiomeMap::generate() {
  // TODO: not sure about the performance of all of that.
  // Maybe the pipeline abstration layer is too much ? In principle, the
  // compiler should be able to resolve this functional-style at compile-time.
  auto step1 = std::bind(&BiomeMap::offsetSimplex, this, _1);
  auto step2 = std::bind(&BiomeMap::offsetVoronoi, this, _1);
  auto step3 = std::bind(&BiomeMap::sampleBiomes, this, _1);
  auto step4 = std::bind(&BiomeMap::blendBiomes, this, _1);
  auto pipeline = make_pipeline(step1, step2, step3, step4);

  voronoi.generateWeighted(ivec2(0), grid);

  map.for_each_parallel([&](vec2 pos, Biome& val) {
    val = pipeline(pos);
  });
}

#include "debug/Debug.hpp"

Biome const& BiomeMap::sampleWeighted(glm::ivec2 pos) const {
  pos += size / 2;
  auto inversion = (abs(pos / size) % 2);
  pos = (size-1) * inversion - abs(pos % size);
  pos = abs(-pos);
  return map[pos];
}


// pipeline step 1: spatial distortion
ivec2 BiomeMap::offsetSimplex(ivec2 pos) {
  vec2 sample(simplexX.simplex2(vec2(pos) / 50.f),
              simplexY.simplex2(vec2(pos) / 50.f));
  sample += 1.0;
  sample *= displacement;

  return pos + ivec2(sample);
}

// pipeline step 2: voronoi splitting (this approximate the distance to chunks borders)
BiomeMap::weightedBiomes_t BiomeMap::offsetVoronoi(ivec2 pos) {
  weightedBiomes_t res;
  auto& sample = grid.at(pos);

  static const std::array<ivec2, 9> posLookup {
    ivec2(-1, -1), ivec2(-1, 0), ivec2(-1, 1),
    ivec2( 0, -1), ivec2( 0, 0), ivec2( 0, 1),
    ivec2( 1, -1), ivec2( 1, 0), ivec2( 1, 1),
  };

  int centerCell = std::min_element(sample.weights.begin(), sample.weights.end()) - sample.weights.begin();
  float centerDist = sample.weights[centerCell];

  float totalWeight = 1.f;

  res.push_back(weightedBiome_t{
    sample.pos + posLookup[centerCell],
    1.f, // this is a temporary value to be normalized later
    nullptr
  });

  for(int i = 0; i < 9; i++) if(i != centerCell) {
    float dist = sample.weights[i] - centerDist;
    if(dist < biomeBlend) {
      float weight = 1.f - dist / biomeBlend;
      totalWeight += weight;
      res.push_back(weightedBiome_t{
        sample.pos + posLookup[i],
        weight, // this is a temporary value to be normalized later
        nullptr
      });
    }
  }

  // normalize coeffs so that the sum of them all is 1
  for(auto& biome : res) {
    biome.weight /= totalWeight;
  }

  return res;
};

// pipeline step 3: sample biome color
BiomeMap::weightedBiomes_t BiomeMap::sampleBiomes(BiomeMap::weightedBiomes_t biomes) {
  // randomized biomes, debugging purposes
  // for(auto& biome : biomes) {
  //   int r = value.sample1D(biome.cellPos);
  //   if(r < INT_MAX * .1) biome.biome = &biomePlains;
  //   else if(r < INT_MAX * .2) biome.biome = &biomeSea;
  //   else if(r < INT_MAX * .4) biome.biome = &biomeDesert;
  //   else if(r < INT_MAX * .6) biome.biome = &biomeToundra;
  //   else if(r < INT_MAX * .8) biome.biome = &biomeHills;
  //   else if(r < INT_MAX     ) biome.biome = &biomeMountains;
  // }
  //
  // return biomes;

  for(auto& biome : biomes) {
    float height = simplexBiome.fractal2(biome.cellPos, heightOctaves);
    float temperature = simplexBiome.fractal2(biome.cellPos + 100, tempOctaves);
    float water = simplexBiome.fractal2(biome.cellPos + 1000, waterOctaves);
    height = abs(height); // adjust between 0 and 1
    temperature = (temperature + 1) / 2;
    water = (water + 1) / 2;

    if (water < .4) {
      biome.biome = &biomeSea;
    }
    // else if (water < ) {
    //   biome.biome = &biomeSea;
    //   // biome = BiomeType::BEACH;
    // }
    else {
      if (height < .6) {
        if (temperature < .3) {
          biome.biome = &biomeToundra;
        }
        else if (temperature < .6) {
          biome.biome = &biomePlains;
        }
        else {
          biome.biome = &biomeDesert;
        }
      }
      else if (height < .75) {
        biome.biome = &biomeHills;
      }
      else {
        biome.biome = &biomeMountains;
      }
    }
  }

  return biomes;
};

// pipeline step 4: blend biomes together
Biome BiomeMap::blendBiomes(BiomeMap::weightedBiomes_t biomes) {
  Biome res = *biomes.front().biome;

  res.elevation = 0.f;
  res.frequencies = {};

  for(auto& weighted : biomes) {

    // blend frequencies
    for(size_t i = 0; i < weighted.biome->frequencies.size(); i++) {
      auto const& freq = weighted.biome->frequencies.at(i);
      if(res.frequencies.size() < i + 1) {
        res.frequencies.push_back({ freq.magnitude * weighted.weight, freq.frequency * weighted.weight });
      }
      else {
        res.frequencies.at(i).magnitude += freq.magnitude * weighted.weight;
        res.frequencies.at(i).frequency += freq.frequency * weighted.weight;
      }
    }

    // blend elevation
    res.elevation += weighted.biome->elevation * weighted.weight;
  }

  return res;
};
