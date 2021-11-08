#include "voronoi.hpp"

#include <glm/glm.hpp>

#include "noise/XXHash.hpp"

using glm::ivec2;
using glm::vec2;

VoronoiNoise::VoronoiNoise(int seed, float cellSize)
    : noise(seed), cellSize(cellSize)
{}

glm::vec2 VoronoiNoise::sample(glm::ivec2 cell) const {
  return vec2(noise.sample<2, 2>(cell)) / vec2(UINT16_MAX);
}

glm::vec2 VoronoiNoise::get(glm::ivec2 cell) const {
  return (vec2(cell) + sample(cell)) * cellSize;
}

glm::ivec2 VoronoiNoise::findCell(vec2 pos) const {
  vec2 gridPos = pos / cellSize;
  ivec2 centerCell = gridPos;
  ivec2 res = centerCell;
  float min_dist = 10.f; // bigger than possible

  ivec2 delta{};
  for (delta.x = -1; delta.x <= 1; delta.x++) {
    for (delta.y = -1; delta.y <= 1; delta.y++) {

      ivec2 otherCell = centerCell + delta;
      vec2 otherGridPos = vec2(otherCell) + sample(otherCell);
      float dist = distance(gridPos, otherGridPos); // TODO: distance squared

      if (dist < min_dist) {
        min_dist = dist;
        res = otherCell;
      }
    }
  }

  return res;
}
