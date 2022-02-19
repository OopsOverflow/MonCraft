#pragma once

#include <glm/glm.hpp>

#include "noise/XXHash.hpp"

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
  VoronoiNoise(int seed, float cellSize);

  glm::vec2 get(glm::ivec2 cell) const;

  glm::ivec2 findCell(glm::vec2 pos) const;

private:
  XXHash noise;
  float cellSize;

  glm::vec2 sample(glm::ivec2 cell) const;
};
