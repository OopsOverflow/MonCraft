#include "BiomeMap.hpp"
#include <functional>
#include <algorithm>
#include <numeric>

using namespace glm;
using namespace std::placeholders;

const float displacement = 40.f;

// spatial distortion octaves
const octaves_t octaves = {
  {displacement * .75f, 0.01f}, // {magnitude, frequency}
  {displacement * .25f, 0.05f},
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
 : voronoi(rand(), (int)ceil(size + 2 * displacement), cellSize, ivec2(0)),
   value(rand()),
   map(size)
{
  simplexX.seed(rand());
  simplexY.seed(rand());
  simplexBiome.seed(rand());

  biomePlains.type = BiomeType::PLAINS;
  biomePlains.elevation = 7;
  biomePlains.distortion = {};
  biomePlains.frequencies = {
    {2.0f, 0.002f}, // {magnitude, frequency}
    {0.5f, 0.007f},
    {0.0f, 0.050f},
    {0.5f, 0.100f},
  };
  biomePlains.surface = BlockType::Grass;
  biomePlains.underLayers = BlockType::Dirt;
  biomePlains.underWaterBlock = BlockType::Dirt;
  biomePlains.trees = 0.05f;
  biomePlains.tallgrass = 0.15f;

  biomeDesert = biomePlains;
  biomeDesert.type = BiomeType::DESERT;
  biomeDesert.surface = BlockType::Sand;
  biomeDesert.underLayers = BlockType::Sand;
  biomeDesert.underWaterBlock = BlockType::Sand;
  biomeDesert.frequencies = {
    {2.0f, 0.002f}, // {magnitude, frequency}
    {1.0f, 0.007f},
    {0.5f, 0.050f},
    {0.0f, 0.100f},
  };
  biomeDesert.elevation = 10;
  biomeDesert.tallgrass = 0;

  biomeSea = biomePlains;
  biomeSea.type = BiomeType::SEA;
  biomeSea.surface = BlockType::Grass;
  biomeSea.underLayers = BlockType::Dirt;
  biomeSea.underWaterBlock = BlockType::Sandstone;
  biomeSea.frequencies = {
  {3.0f, 0.002f}, // {magnitude, frequency}
  {1.5f, 0.007f},
  {0.5f, 0.050f},
  {0.5f, 0.100f},
  };
  biomeSea.elevation = -40;
  biomeSea.tallgrass = 0;

  biomeToundra = biomePlains;
  biomeToundra.type = BiomeType::TOUNDRA;
  biomeToundra.surface = BlockType::Snow;
  biomeToundra.underLayers = BlockType::Dirt;
  biomeToundra.underWaterBlock = BlockType::Dirt;
  biomeToundra.tallgrass = 0.03f;
  biomeToundra.elevation = 12;

  biomeHills = biomePlains;
  biomeHills.type = BiomeType::HILLS;
  biomeHills.surface = BlockType::Grass;
  biomeHills.underLayers = BlockType::Dirt;
  biomeHills.underWaterBlock = BlockType::Dirt;
  biomeHills.frequencies = {
    {2.0f, 0.002f}, // {magnitude, frequency}
    {0.5f, 0.007f},
    {0.0f, 0.050f},
    {0.0f, 0.100f},
  };
  // biomeHills.tallgrass = 0;
  biomeHills.elevation = 15;

  biomeMountains = biomePlains;
  biomeMountains.type = BiomeType::MOUNTAINS;
  biomeMountains.surface = BlockType::Stone;
  biomeMountains.underLayers = BlockType::Stone;
  biomeMountains.underWaterBlock = BlockType::Stone;
  biomeMountains.frequencies = {
    {7.0f, 0.002f}, // {magnitude, frequency}
    {0.0f, 0.007f},
    {0.0f, 0.050f},
    {0.0f, 0.100f},
  };
  biomeMountains.elevation = 110;
  biomeMountains.tallgrass = 0;

  biomeForest = biomePlains;
  biomeForest.type = BiomeType::FOREST;
  biomeForest.surface = BlockType::Grass;
  biomeForest.underLayers = BlockType::Dirt;
  biomeForest.underWaterBlock = BlockType::Dirt;
  biomeForest.frequencies = {
    {3.0f, 0.002f}, // {magnitude, frequency}
    {1.5f, 0.007f},
    {0.0f, 0.050f},
    {0.0f, 0.100f},
  };
  // biomeHills.tallgrass = 0;
  biomeForest.elevation = 10;
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
  // return pos; // to disable distortion, uncomment this ;)
  vec2 sample(simplexX.fractal2(vec2(pos), octaves),
              simplexY.fractal2(vec2(pos), octaves));

  return pos + ivec2(sample);
}

// pipeline step 2: voronoi splitting (this approximate the distance to chunks borders)
BiomeMap::weightedBiomes_t BiomeMap::offsetVoronoi(ivec2 ipos) {
    weightedBiomes_t res;

    vec2 pos = ipos;
    ivec2 mainCell = voronoi.findCell(pos);
    vec2 mainPos = voronoi.get(mainCell);

    float thres = cellSize * 2;

    float dist = distance(pos, mainPos);
    if(dist < thres) {
      float weight = (float)pow(0.001 * (thres - dist), 4);
      res.push_back({
        mainCell,
        weight,
        nullptr
      });
    }

    ivec2 delta{};
    for (delta.x = -2; delta.x <= 2; delta.x++) {
      for (delta.y = -2; delta.y <= 2; delta.y++) {

        if(delta == ivec2(0)) continue;
        ivec2 cell = mainCell + delta;
        if(!voronoi.isInRange(cell)) continue;

        vec2 otherPos = voronoi.get(cell);
        float dist = distance(pos, otherPos);
        if(dist < thres) {
          float weight = (float)pow(0.001 * (thres - dist), 4);
          res.push_back({
            cell,
            weight,
            nullptr
          });
        }

      }
    }

    if(res.size() == 0) { // BUG
      res.push_back({
        mainCell,
        0.f,
        nullptr
      });
    }

    // normalize weights (sum = 1)
    float totalWeight = 0;
    for(auto const& biome: res) {
      totalWeight += biome.weight;
    }
    for(auto& biome: res) {
      biome.weight /= totalWeight;
    }

    return res;
};


pixel_t BiomeMap::teststep(BiomeMap::weightedBiomes_t biomes) {
  pixel_t col(0);

  for(size_t i = 0; i < biomes.size(); i++) {
    col += vec3(value.sample<2, 3>(biomes.at(i).cellPos)) * biomes.at(i).weight;
  }

  return col;
}

// pipeline step 3: sample biome color
BiomeMap::weightedBiomes_t BiomeMap::sampleBiomes(BiomeMap::weightedBiomes_t biomes) {
  // randomized biomes, debugging purposes
  // for(auto& biome : biomes) {
  //   int r = value.sample1D(biome.cellPos);
  //   if(r < INT_MAX * .1) biome.biome = &biomePlains;
  //   else if(r < INT_MAX * .2) biome.biome = &biomeSea;
  //   else if(r < INT_MAX * .3) biome.biome = &biomeForest;
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

    if (water < .2) {
      biome.biome = &biomeSea;
    }
    // else if (water < ) {
    //   biome.biome = &biomeSea;
    //   // biome = BiomeType::BEACH;
    // }
    else {
      if (height < .6) {
        if (temperature < .2) {
          biome.biome = &biomeToundra;
        }
        else if (temperature < .4) {
          biome.biome = &biomePlains;
        }
        else if (temperature < .55) {
            biome.biome = &biomeForest;
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
        res.frequencies.push_back({
          freq.magnitude * weighted.weight * globalAmplitude,
          freq.frequency * weighted.weight  * globalFrequency
        });
      }
      else {
        res.frequencies.at(i).magnitude += freq.magnitude * weighted.weight * globalAmplitude;
        res.frequencies.at(i).frequency += freq.frequency * weighted.weight * globalFrequency;
      }
    }

    // blend elevation
    res.elevation += weighted.biome->elevation * weighted.weight;
  }


  return res;
};
