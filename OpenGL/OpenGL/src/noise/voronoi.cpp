#include "voronoi.hpp"
#include <memory>

using glm::ivec2;
using glm::vec2;

VoronoiNoise::VoronoiNoise(int seed, int gridSize)
    : noise(ValueNoise(seed)),
      gridSize(gridSize) {}

// ivec2 VoronoiNoise::sample(ivec2 pos) {
//   ivec2 ipos = floor(vec2(pos / gridSize));
//   ivec2 res = ipos;
//
//   float min_dist = 10 * gridSize; // bigger than possible
//
//   for (int di = -1; di <= 1; di++)
//     for (int dj = -1; dj <= 1; dj++) {
//       ivec2 ipos2 = ipos + ivec2(di, dj);
//       vec2 rpos2 = vec2(ipos2) + vec2(noise.sample2D(ipos2)) / vec2(UINT16_MAX);
//       ivec2 pos2 = rpos2 * vec2(gridSize);
//       float dist = glm::length(vec2(pos2 - pos));
//
//       if (dist < min_dist) {
//       	min_dist = dist;
//       	res = ipos2;
//       }
//     }
//
//   return res;
// }


void VoronoiNoise::generate(ivec2 pos, Grid<glm::vec2>& map) const {
  float fGridSize = (float)gridSize;
  ivec2 istart = floor(vec2(pos) / fGridSize);
  istart -= 1;
  ivec2 iend = ceil(vec2(pos + map.size()) / fGridSize);
  iend += 1;
  ivec2 isize = iend - istart;
  ivec2 offset = pos % gridSize;

  Grid<glm::vec2> grid(isize.x, isize.y);
  grid.for_each([&](ivec2 ipos, vec2& val) {
    val = vec2(noise.sample2D(istart + ipos)) / vec2(UINT16_MAX);
  });

  map.for_each_parallel([&](vec2 pos, vec2& val) {
    pos += offset;
    ivec2 ipos = floor(pos / fGridSize);
    ivec2 res = ipos;
    float min_dist = 10 * gridSize; // bigger than possible

    for (int di = -1; di <= 1; di++) {
      for (int dj = -1; dj <= 1; dj++) {

        ivec2 ipos2 = ipos + ivec2(di, dj);
        vec2 rpos2 = vec2(ipos2) + grid.at(ipos2 + ivec2(1));
        vec2 pos2 = rpos2 * fGridSize;
        float dist = distance(pos2, pos);

        if (dist < min_dist) {
          min_dist = dist;
          res = ipos2;
        }
      }
    }

    val = res;
  });
}
