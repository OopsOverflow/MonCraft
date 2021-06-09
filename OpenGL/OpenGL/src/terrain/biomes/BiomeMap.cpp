#include "BiomeMap.hpp"
#include <functional>
#include <algorithm>
#include <numeric>

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
 : voronoi(rand(), gridSize, ivec2(0)),
   value(rand()),
   map(size), test(size)
{
  simplexX.seed(rand());
  simplexY.seed(rand());
  simplexBiome.seed(rand());

  biomePlains.type = BiomeType::PLAINS;
  biomePlains.elevation = 7;
  biomePlains.distortion = {};
  biomePlains.frequencies = {
    {2.0f, 0.002f}, // {magnitude, frequency}
    {1.5f, 0.007f},
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
    {1.5f, 0.007f},
    {1.0f, 0.050f},
    {0.0f, 0.100f},
  };
  biomeDesert.elevation = 10;
  biomeDesert.tallgrass = 0;

  biomeSea = biomePlains;
  biomeSea.type = BiomeType::SEA;
  biomeSea.surface = BlockType::Sand;
  biomeSea.underLayers = BlockType::Sand;
  biomeSea.underWaterBlock = BlockType::Sandstone;
  biomeSea.frequencies = {
  {3.0f, 0.002f}, // {magnitude, frequency}
  {1.5f, 0.007f},
  {0.5f, 0.050f},
  {0.5f, 0.100f},
  };
  biomeSea.elevation = -10;
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
    {6.0f, 0.002f}, // {magnitude, frequency}
    {2.5f, 0.007f},
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
    {20.0f, 0.002f}, // {magnitude, frequency}
    {18.0f, 0.007f},
    {2.0f, 0.050f},
    {1.5f, 0.100f},
  };
  biomeMountains.elevation = 20;
  biomeMountains.tallgrass = 0;

  biomeForest = biomePlains;
  biomeForest.type = BiomeType::FOREST;
  biomeForest.surface = BlockType::Grass;
  biomeForest.underLayers = BlockType::Dirt;
  biomeForest.underWaterBlock = BlockType::Dirt;
  biomeForest.frequencies = {
    {3.0f, 0.002f}, // {magnitude, frequency}
    {2.5f, 0.007f},
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

  auto teststep = std::bind(&BiomeMap::teststep, this, _1);
  auto testpipeline = make_pipeline(step1, step2, teststep);

  map.for_each_parallel([&](vec2 pos, Biome& val) {
    val = pipeline(pos);
  });

  test.for_each_parallel([&](vec2 pos, pixel_t& val) {
    val = testpipeline(pos);
  });

  testtex.generate(test);
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
  return pos; // to disable distortion, uncomment this ;)
  vec2 sample(simplexX.simplex2(vec2(pos) * frequency),
              simplexY.simplex2(vec2(pos) * frequency));
  sample += 1.0;
  sample *= displacement;

  return pos + ivec2(sample);
}

float f(float x, float a) {
  // return x;
  return -(((2*a-2)*sqrt((1-2*a)*x+a*a)+(1-2*a)*x-2*a*a+2*a)/(4*a*a-4*a+1));
}

float smooth(float x, float a) {
  return -f(-x+1.f, a) + 1.f;
  // return f(x, a);
}


float sign (vec2 p1, vec2 p2, vec2 p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle (vec2 pt, vec2 v1, vec2 v2, vec2 v3) {
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}


// pipeline step 2: voronoi splitting (this approximate the distance to chunks borders)
BiomeMap::weightedBiomes_t BiomeMap::offsetVoronoi(ivec2 ipos) {
  weightedBiomes_t res;

  vec2 pos = ipos;
  ivec2 mainCell = voronoi.findCell(pos);
  vec2 mainPos = voronoi.get(mainCell);

  static const std::array<ivec2, 8> neighbors = {
    ivec2
    {-1, -1}, {-1,  0}, {-1,  1}, { 0,  1},
    { 1,  1}, { 1,  0}, { 1, -1}, { 0, -1},
  };
  int found = 0;

  for(int i = 0; i < neighbors.size(); i++) {

    ivec2 otherCell1 = mainCell + neighbors[i];
    ivec2 otherCell2 = mainCell + neighbors[(i + 1) % 8];
    vec2 otherPos1 = voronoi.get(otherCell1);
    vec2 otherPos2 = voronoi.get(otherCell2);

    if(found) {
    }

    else if(PointInTriangle(pos, mainPos, otherPos1, otherPos2)) {

      float a = (otherPos1 - otherPos2).y * (mainPos - otherPos2).x + (otherPos2 - otherPos1).x * (mainPos - otherPos2).y;
      vec2 b = pos - otherPos2;

      float w1, w2, w3;
      float c = (otherPos1 - otherPos2).y * b.x + (otherPos2 - otherPos1).x * b.y;
      float d = (otherPos2 - mainPos).y * b.x + (mainPos - otherPos2).x * b.y;

      w1 = c / a;
      w2 = d / a;
      w3 = 1.f - w1 - w2;

      res.push_back({
        mainCell,
        w1,
        nullptr
      });
      res.push_back({
        otherCell1,
        w2,
        nullptr
      });
      res.push_back({
        otherCell2,
        w3,
        nullptr
      });

      found = i+1;
    }
  }

  if(res.size() == 0) {
      res.push_back({
        mainCell,
        0.f,
        nullptr
      });
  }

  // normalize weights (sum = 1)
  // float totalWeight = 0;
  // for(auto const& biome: res) {
  //   totalWeight += biome.weight;
  // }
  // for(auto& biome: res) {
  //   biome.weight /= totalWeight;
  // }

  return res;
};


pixel_t BiomeMap::teststep(BiomeMap::weightedBiomes_t biomes) {
  pixel_t col(0);

  for(int i = 0; i < std::min(10ul, biomes.size()); i++) {
    col += vec3(value.sample3D(biomes.at(i).cellPos)) * biomes.at(i).weight;
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

    if (water < .4) {
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
        else if (temperature < .5) {
          biome.biome = &biomePlains;
        }
        else if (temperature < .65) {
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
