#include "value.hpp"
#include "../debug.hpp"

using glm::ivec2;

// --------- a (probably very bad), but fast LCG.

int ValueNoise::rand (unsigned long prn) const
{
  prn = prn * 1103515245 + 12345;
  return (unsigned int)(prn/65536) % INT_MAX;
}

ValueNoise::ValueNoise(int seed) : seed(seed) {
}

int ValueNoise::sample1D(const ivec2 &pos) const {
  int a = rand(pos.x * seed + 54321);
  int b = rand(pos.y + 12345);
  return rand(a ^ b);
}

glm::vec<2, uint16_t> ValueNoise::sample2D(const ivec2 &pos) const {
  int h = sample1D(pos);
  glm::vec<2, uint16_t> res{
      h & UINT16_MAX,
      (h >> 16) & UINT16_MAX,
  };
  return res;
}

glm::vec<3, uint8_t> ValueNoise::sample3D(const ivec2 &pos) const {
  int h = sample1D(pos);
  glm::vec<3, uint8_t> res{
      h & UINT8_MAX,
      (h >> 8) & UINT8_MAX,
      (h >> 16) & UINT8_MAX,
  };
  return res;
}
