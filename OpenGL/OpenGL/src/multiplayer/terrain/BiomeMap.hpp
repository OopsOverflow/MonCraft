#pragma once

#include <map>
#include <utility>
#include <functional>

#include "noise/Noise.hpp"
#include "Biome.hpp"
// #include "Plains.hpp"
// #include "Sea.hpp"
// #include "Desert.hpp"


class BiomeMap {

public:
  BiomeMap();

  /**
   * Samples a position in the biome map and returns a Biome description that
   * possibly blends multiple biomes if near an edge.
   */
  Biome sampleWeighted(glm::ivec2 pos) const;

private:
    float cellSize = 150.f;         // voronoi single cell size
    float blendSmoothness = 0.4f;   // amount of smoothing to apply
    float biomeBlend = 40.f; // distance to smooth biomes borders
    float globalFrequency = .5f;   // adjust the frequencies overall (high values gives)
    float globalAmplitude = 5.2f;  // adjust the amplitude overall (high values gives AMPLIFIED terrain)

    // noises
    SimplexNoise simplexX;       // samples space in x direction
    SimplexNoise simplexY;       // samples space in y direction
    SimplexNoise simplexBiome;   // samples biome type
    VoronoiNoise voronoi;        // voronoi cells
    XXHash value;            // simple random noise

    // helper types
    struct weightedBiome_t {
      glm::ivec2 cellPos;
      float weight; // weight between 0 and 1
      Biome* biome;
    };
    using weightedBiomes_t = std::vector<weightedBiome_t>;

    // pipeline functions
    // note: the compiler should be able to optimize copies with RVO
    glm::ivec2 offsetSimplex(glm::ivec2 pos);
    weightedBiomes_t offsetVoronoi(glm::ivec2 pos);
    weightedBiomes_t sampleBiomes(weightedBiomes_t biomes);
    Biome blendBiomes(weightedBiomes_t biomes);
    pixel_t bitmapStep(BiomeMap::weightedBiomes_t biomes);
    std::function<Biome(glm::ivec2)> generator;

    Biome biomePlains;
    Biome biomeSea;
    Biome biomeDesert;
    Biome biomeToundra;
    Biome biomeHills;
    Biome biomeMountains;
    Biome biomeForest;
};
