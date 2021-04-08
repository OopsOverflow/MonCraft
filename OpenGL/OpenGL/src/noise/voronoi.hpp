#pragma once

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
  VoronoiNoise(int seed, int gridSize);

  /**
   * Samples a point in 2D space and returns the grid index of the voronoi cell
   * at this point.
   */
  // glm::ivec2 sample(glm::ivec2 pos);


  /**
   * Convenience method to generate a bitmap out of the voronoi diagram.
   */
  void generate(glm::ivec2 pos, Grid<glm::vec2> &map) const;

private:
  ValueNoise noise;
  int gridSize;
};
