#include "value.hpp"

#include <glm/glm.hpp>

using glm::ivec2;

// --------- a (probably very bad), but fast LCG.

uint32_t ValueNoise::rand (uint64_t prn) const
{
  prn = prn * 1103515245 + 12345;
  return (uint32_t)(prn/65536) % INT32_MAX;
}

ValueNoise::ValueNoise(uint32_t seed) : seed(rand(seed)) {
}

uint32_t ValueNoise::sample1D(const ivec2 &pos) const {
  uint32_t a = rand(pos.x * seed + 54321);
  uint32_t b = rand(pos.y + 12345);
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
