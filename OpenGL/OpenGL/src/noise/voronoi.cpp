#include "voronoi.hpp"
#include <memory>

using glm::ivec2;
using glm::vec2;

VoronoiNoise::VoronoiNoise(int seed, int gridSize, ivec2 offset)
    : noise(ValueNoise(seed)),
      gridSize(gridSize), fGridSize(gridSize),
      grid(gridSize + 2)
{
  grid.for_each([&](ivec2 ipos, vec2& val) {
    val = vec2(noise.sample2D(offset + ipos)) / vec2(UINT16_MAX);
  });
}

glm::vec2 VoronoiNoise::get(glm::ivec2 cell) const {
    return (vec2(cell) + grid.at(cell + 1)) * fGridSize;
}

glm::ivec2 VoronoiNoise::findCell(vec2 pos) const {
  vec2 gridPos = pos / fGridSize;
  ivec2 centerCell = gridPos;
  ivec2 res = centerCell;
  float min_dist = 10.f; // bigger than possible

  ivec2 delta{};
  for (delta.x = -1; delta.x <= 1; delta.x++) {
    for (delta.y = -1; delta.y <= 1; delta.y++) {

      ivec2 otherCell = centerCell + delta;
      vec2 otherGridPos = vec2(otherCell) + grid.at(otherCell + 1);
      float dist = distance(gridPos, otherGridPos); // TODO: distance squared

      if (dist < min_dist) {
        min_dist = dist;
        res = otherCell;
      }
    }
  }

  return res;
}
