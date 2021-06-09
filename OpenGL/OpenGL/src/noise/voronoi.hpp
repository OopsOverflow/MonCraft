#pragma once

#include <array>
#include "bitmap.hpp"
#include "value.hpp"

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
  VoronoiNoise(int seed, int gridSize, glm::ivec2 offset);

  glm::vec2 get(glm::ivec2 cell) const;

  glm::ivec2 findCell(glm::vec2 pos) const;


private:
  ValueNoise noise;
  int gridSize;
  float fGridSize;
  Grid<glm::vec2> grid;
};
