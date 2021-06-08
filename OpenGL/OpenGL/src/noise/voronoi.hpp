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

  /**
   * Samples a point in 2D space and returns the grid index of the voronoi cell
   * at this point.
   */
  // glm::ivec2 sample(glm::ivec2 pos);


  /**
   * Generates the grid voronoi diagram.
   */
  void generate(Grid<glm::vec2> &map) const;


  struct weightedSample_t {
    glm::ivec2 pos;
    std::array<float, 9> weights; // see operator[]
    float operator[](glm::ivec2 off) { // off is between (-1, -1) and (1, 1) (offset from pos)
      return weights[(off.x + 1) * 3 + off.y + 1];
    }
  };

  /**
   * Generates the grid voronoi, giving the weights of the four closest cells
   * based on distance from the cell's center to the sampling point.
   */
  void generateWeighted(glm::ivec2 pos, Grid<weightedSample_t> &map, float margin) const;

private:

  glm::ivec2 findCell(glm::vec2 pos) const;
  bool isNear(glm::vec2 pos, glm::ivec2 cell, float margin) const;

  ValueNoise noise;
  int gridSize;
  Grid<glm::vec2> grid;
};
