#include "voronoi.hpp"
#include <memory>

using glm::ivec2;
using glm::vec2;

VoronoiNoise::VoronoiNoise(int seed, int gridSize, ivec2 offset)
    : noise(ValueNoise(seed)),
      gridSize(gridSize),
      grid(gridSize + 2)
{
  grid.for_each([&](ivec2 ipos, vec2& val) {
    val = vec2(noise.sample2D(offset + ipos)) / vec2(UINT16_MAX);
  });
}

glm::ivec2 VoronoiNoise::findCell(glm::vec2 pos) const {
  float fGridSize = (float)gridSize;

  ivec2 ipos = pos / fGridSize;
  ivec2 res = ipos;
  float min_dist = 10.f * gridSize; // bigger than possible

  ivec2 dpos{};
  for (dpos.x = -1; dpos.x <= 1; dpos.x++) {
    for (dpos.y = -1; dpos.y <= 1; dpos.y++) {

      ivec2 ipos2 = ipos + dpos;
      vec2 rpos2 = vec2(ipos2) + grid.at(ipos2 + 1);
      vec2 pos2 = rpos2 * fGridSize;
      float dist = distance(pos2, pos);

      if (dist < min_dist) {
        min_dist = dist;
        res = ipos2;
      }
    }
  }

  return res;
}

void VoronoiNoise::generate(Grid<glm::vec2>& map) const {
  map.for_each_parallel([&](vec2 pos, vec2& val) {
    val = findCell(pos);
  });
}

#include "debug/Debug.hpp"


bool VoronoiNoise::isNear(vec2 pos, ivec2 cell, float margin) const {
  float fGridSize = (float)gridSize;

  vec2 min_rpos = vec2(cell) + grid.at(cell + 1);
  vec2 rpos = pos / fGridSize;

  // compute weights
  ivec2 dpos{};
  for (dpos.x = -1; dpos.x <= 1; dpos.x++) {
      for (dpos.y = -1; dpos.y <= 1; dpos.y++) {

          float dist;
          if (dpos != ivec2(0)) {
            ivec2 ipos2 = cell + dpos;
            vec2 rpos2 = vec2(ipos2) + grid.at(ipos2 + 1);
            dist = dot((min_rpos + rpos2) / 2.f - rpos, normalize(rpos2 - min_rpos));
            if(dist * fGridSize + margin < 0) {
              return false;
            }
          }
      }
  }

  return true;
}

void VoronoiNoise::generateWeighted(ivec2 pos, Grid<weightedSample_t>& map, float margin) const {
  float fGridSize = (float)gridSize;

  map.for_each_parallel([&](vec2 pos, weightedSample_t& val) {
      vec2 rpos = pos / fGridSize;

      // find cell we are in
      ivec2 cell = findCell(pos);
      vec2 min_rpos = vec2(cell) + grid.at(cell + 1);
      val.pos = cell;

      // compute weights
      ivec2 dpos{};
      int i = 0;
      for (dpos.x = -1; dpos.x <= 1; dpos.x++) {
          for (dpos.y = -1; dpos.y <= 1; dpos.y++) {

              float dist;
              if (dpos == ivec2(0)) {
                dist = 0;
              }
              else {
                ivec2 ipos2 = val.pos + dpos;
                vec2 rpos2 = vec2(ipos2) + grid.at(ipos2 + 1);
                dist = dot((min_rpos + rpos2) / 2.f - rpos, normalize(rpos2 - min_rpos));
                if(!isNear(pos, ipos2, margin)) dist = 10000;
              }

              val.weights[i++] = dist * fGridSize;
          }
      }

  });
}
