#pragma once

#include <map>
#include <utility>

#include "noise/Noise.hpp"
#include "Biome.hpp"
// #include "Plains.hpp"
// #include "Sea.hpp"
// #include "Desert.hpp"


class BiomeMap {

public:
  BiomeMap();


  /**
   * Generates the whole biome map.
   * This is to be called once. It is expensive.
   */
  void generate();

  /**
   * Samples a position in the biome map and returns a Biome description that
   * possibly blends multiple biomes if near an edge.
   */
  Biome const& sampleWeighted(glm::ivec2 pos) const;
private:
    // biome gen tuning
    int size = 1000;           // generated texture size
    int gridSize = 100;        // voronoi single cell size
    int displacement = 10;     // spatial displacement (distortion)
    float biomeBlend = 15.f;   // distance to smooth biomes borders

    // noises
    SimplexNoise simplexX;       // samples space in x direction
    SimplexNoise simplexY;       // samples space in y direction
    SimplexNoise simplexBiome;   // samples biome type
    VoronoiNoise voronoi;        // voronoi cells
    ValueNoise value;            // simple random noise

    // helper types
    struct weightedBiome_t {
      glm::ivec2 cellPos;
      float weight; // weight between 0 and 1
      Biome* biome;
    };
    using weightedBiomes_t = std::vector<weightedBiome_t>;

    // bitmaps
    Grid<VoronoiNoise::weightedSample_t> grid;  // voronoi
    Grid<Biome> map;      // final Biome Map

    // pipeline functions
    // note: the compiler should be able to optimize copies with RVO
    glm::ivec2 offsetSimplex(glm::ivec2 pos);
    weightedBiomes_t offsetVoronoi(glm::ivec2 pos);
    weightedBiomes_t sampleBiomes(weightedBiomes_t biomes);
    Biome blendBiomes(weightedBiomes_t biomes);

    Biome biomePlains;
    Biome biomeSea;
    Biome biomeDesert;
    Biome biomeToundra;
    Biome biomeHills;
    Biome biomeMountains;
};
