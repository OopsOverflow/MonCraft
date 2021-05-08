#pragma once

#include <vector>
#include "bitmap.hpp"

/**
 * A procedural noise sampler for perlin and simplex noise.
 *
 * Sampled values are in the range [-1, 1].
 *
 * credit: https://github.com/josephg/noisejs/blob/master/perlin.js
 * translated from javascript.
 */

struct octave_t {
  float magnitude;
  float frequency;
};
using octaves_t = std::vector<octave_t>;

class SimplexNoise
{
public:
  SimplexNoise();

  void seed(int s);
  float simplex2(glm::vec2 pos) const;
  float simplex3(glm::vec3 pos);
  float perlin2(glm::vec2 pos);
  float perlin3(glm::vec3 pos);
  float fractal2(glm::vec2 pos, octaves_t const& octaves) const;

private:
  struct Grad
  {
    int x, y, z;

    float dot2(float x, float y)
    {
      return this->x * x + this->y * y;
    }
    float dot3(float x, float y, float z)
    {
      return this->x * x + this->y * y + this->z * z;
    }
  };

  int perm[512];
  Grad gradP[512];
  static const Grad grad3[12];
};
