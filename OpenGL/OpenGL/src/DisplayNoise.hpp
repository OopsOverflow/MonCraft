#pragma once

#include <chrono>
#include <functional>
#include <map>
#include <vector>
#include <future>
#include <iostream>
#include <memory>
#include <thread>
using namespace std::chrono;

#include "Shader.hpp"
#include "Viewport.hpp"
#include "Noise.hpp"

using namespace std::placeholders;
using glm::ivec2;
using glm::vec2;
using glm::vec3;
using glm::vec4;

class DisplayNoise {

private:
  float speed = 50.f;        // moving speed when displaying new frames
  int size = 1000;           // generated texture size
  int gridSize = size / 100; // voronoi grid size
  int displacement = 10;     // spatial displacement (distortion)
  float i = 0;               // frame counter

  // noises
  SimplexNoise simplexX;     // samples space in x direction
  SimplexNoise simplexY;     // samples space in y direction
  SimplexNoise simplexBiome; // samples biome type
  VoronoiNoise voronoi;      // voronoi cells
  ValueNoise value;          // simple random noise

  // bitmaps
  Grid<vec2> grid; // voronoi
  Bitmap map;      // final texture bitmap
  BitmapTex tex;   // texture

  std::future<void> map_task;     // async task to draw the bitmap
  std::function<void()> make_map; // function to draw the bitmap

  // spatial distortion octaves
  const octaves_t octaves = {
    {.65, 0.004}, // {magnitude, frequency}
    {.20, 0.008},
    {.10, 0.050},
    {.05, 0.100},
  };

  // temperature / dryness octaves
  octaves_t tempOctaves = {
    {1, .08f},
  };

  // altitude octaves
  octaves_t heightOctaves = {
    {1, 1.f},
  };

  // moisture/water level octaves
  octaves_t waterOctaves = {
    {1, 0.1f},
  };

  // biomes rgb colors
  enum class biomes { ICE, SEA, BEACH, TOUNDRA, PLAINS, DESERT, HILLS, MOUNTAINS };
  std::map<biomes, pixel_t> biomesColors = {
    {biomes::ICE, {192, 235, 214}},
    {biomes::SEA, {57, 105, 255}},
    {biomes::BEACH, {226, 228, 131}},
    {biomes::TOUNDRA, {101, 195, 121}},
    {biomes::PLAINS, {25, 222, 17}},
    {biomes::DESERT, {218, 222, 86}},
    {biomes::HILLS, {49, 147, 56}},
    {biomes::MOUNTAINS, {243, 251, 247}},
  };

  // pipeline step 1: spatial distortion
  std::function<ivec2(ivec2)> offsetSimplex = [&](ivec2 pos) {
    vec2 sample(simplexX.simplex2(vec2(pos) / 50.f),
                simplexY.simplex2(vec2(pos) / 50.f));
    sample += 1.0;
    sample *= displacement;

    return pos + ivec2(sample);
  };

  // pipeline step 2: voronoi splitting
  std::function<ivec2(ivec2)> offsetVoronoi = [&](ivec2 pos) {
    pos -= ivec2(i);
    return grid.at(pos) + floor(vec2(i) / (float)gridSize);
  };

  // pipeline step 3: sample biome color
  std::function<pixel_t(ivec2)> sampleBiome = [&](ivec2 pos) {
    float height = simplexBiome.fractal2(pos, heightOctaves);
    float temperature = simplexBiome.fractal2(pos + 100, tempOctaves);
    float water = simplexBiome.fractal2(pos + 1000, waterOctaves);
    height = abs(height);
    temperature = (temperature + 1) / 2;
    water = (water + 1) / 2;

    pixel_t col;

    if (.3 >= water) {
      col = biomesColors[biomes::SEA];
    }
    else if (.35 >= water) {
      col = biomesColors[biomes::BEACH];
    }
    else {
      if (height < .8) {
        if (temperature < .3)
          col = biomesColors[biomes::TOUNDRA];
        else if (temperature < .7)
          col = biomesColors[biomes::PLAINS];
        else
          col = biomesColors[biomes::DESERT];
      }
      else if (height < .9)
        col = biomesColors[biomes::HILLS];
      else
        col = biomesColors[biomes::MOUNTAINS];
      }

    return col;
  };

  decltype(make_pipeline(offsetSimplex, offsetVoronoi, sampleBiome)) pipeline =
           make_pipeline(offsetSimplex, offsetVoronoi, sampleBiome);

public:
  DisplayNoise()
      : voronoi(20, gridSize), value(10),
        grid(size + 2 * displacement, size + 2 * displacement),
        map(size, size) {
    simplexX.seed(10);
    simplexY.seed(20);
    simplexBiome.seed(30);

    make_map = [&] {
      voronoi.generate({i, i}, grid);
      map.for_each_parallel([&](vec2 pos, pixel_t &val) {
        val = pipeline(pos + vec2(i));
      });
    };

    make_map();
    tex.generate(map);
    map_task = std::async(std::launch::async, make_map);
  }

  void draw() {
    auto status = map_task.wait_for(0ms);
    if (status == std::future_status::ready) {
      tex.generate(map);
      std::cout << "frame drawn" << std::endl;
      i += size / speed;
      map_task = std::async(std::launch::async, make_map);
    }
    else {
      // std::cout << "Thread still running" << std::endl;
    }

    tex.draw();
  }
};
