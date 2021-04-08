#pragma once

/**
 * A procedural noise sampler for perlin and simplex noise.
 *
 * Sampled values are in the range [-1, 1].
 *
 * credit: https://github.com/josephg/noisejs/blob/master/perlin.js
 * translated from javascript.
 */

class SimplexNoise
{
public:
  SimplexNoise();

  void seed(int s);
  float simplex2(float x, float y);
  float simplex3(float x, float y, float z);
  float perlin2(float x, float y);
  float perlin3(float x, float y, float z);

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
