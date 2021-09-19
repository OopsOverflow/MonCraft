#pragma once

#include <array>
#include "bitmap.hpp"
#include "XXHash.hpp"

/**
 * A grid based voronoi noise.
 * For a given point, finds the 2D index of the voronoi cell it belongs to.
 */

class VoronoiNoise
{
public:

  /**
   * Constructs a voronoi noise generator.
   * samples are reproducible.
   */
  VoronoiNoise(int seed, int size, float cellSize, glm::ivec2 offset);

  glm::vec2 get(glm::ivec2 cell) const;

  glm::ivec2 findCell(glm::vec2 pos) const;


private:
  XXHash noise;
  int size;
  float cellSize;
  Grid<glm::vec2> grid;
};
