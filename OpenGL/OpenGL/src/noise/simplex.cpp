// credit: https://github.com/josephg/noisejs/blob/master/perlin.js
// translated from javascript.

/*
 * A speed-improved perlin and simplex noise algorithms for 2D.
 *
 * Based on example code by Stefan Gustavson (stegu@itn.liu.se).
 * Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
 * Better rank ordering method by Stefan Gustavson in 2012.
 * Converted to Javascript by Joseph Gentle.
 *
 * Version 2012-03-09
 *
 * This code was placed in the public domain by its original author,
 * Stefan Gustavson. You may use it as you see fit, but
 * attribution is appreciated.
 *
 */

#include "simplex.hpp"

#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;

const float F2 = 0.5f * (sqrt(3.f) - 1.f);
const float G2 = (3.f - sqrt(3.f)) / 6.f;

const float F3 = 1.f / 3.f;
const float G3 = 1.f / 6.f;

const SimplexNoise::Grad SimplexNoise::grad3[] = {
  Grad{1, 1, 0}, Grad{-1, 1, 0}, Grad{1, -1, 0}, Grad{-1, -1, 0},
  Grad{1, 0, 1}, Grad{-1, 0, 1}, Grad{1, 0, -1}, Grad{-1, 0, -1},
  Grad{0, 1, 1}, Grad{0, -1, 1}, Grad{0, 1, -1}, Grad{0, -1, -1}
};

const int p[] = {
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,
    36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120,
    234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
    134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
    230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,
    1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130,
    116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250,
    124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227,
    47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44,
    154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
    98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
    251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235,
    249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,
    115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29,
    24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

SimplexNoise::SimplexNoise()
    : perm(),
      gradP()
{}

void SimplexNoise::seed(int seed)
{
  if (seed < 256)
  {
    seed |= seed << 8;
  }

  for (size_t i = 0; i < 256; i++)
  {
    int v;
    if (i & 1)
    {
      v = p[i] ^ (seed & 255);
    }
    else
    {
      v = p[i] ^ ((seed >> 8) & 255);
    }

    perm[i] = perm[i + 256] = v;
    gradP[i] = gradP[i + 256] = grad3[v % 12];
  }
}

float fade(float t)
{
  return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float a, float b, float t)
{
  return (1 - t) * a + t * b;
}

float SimplexNoise::perlin2(vec2 pos) const
{
  // Find unit grid cell containing point
  int X = (int)floor(pos.x);
  int Y = (int)floor(pos.y);
  // Get relative xy coordinates of point within that cell
  pos.x = pos.x - X;
  pos.y = pos.y - Y;
  // Wrap the integer cells at 255 (smaller integer period can be introduced here)
  X = X & 255;
  Y = Y & 255;

  // Calculate noise contributions from each of the four corners
  float n00 = gradP[X + perm[Y]].dot2(pos.x, pos.y);
  float n01 = gradP[X + perm[Y + 1]].dot2(pos.x, pos.y - 1);
  float n10 = gradP[X + 1 + perm[Y]].dot2(pos.x - 1, pos.y);
  float n11 = gradP[X + 1 + perm[Y + 1]].dot2(pos.x - 1, pos.y - 1);

  // Compute the fade curve value for x
  float u = fade(pos.x);

  // Interpolate the four results
  return lerp(
      lerp(n00, n10, u),
      lerp(n01, n11, u),
      fade(pos.y));
}

float SimplexNoise::perlin3(vec3 pos) const
{
  // Find unit grid cell containing point
  int X = (int)floor(pos.x);
  int Y = (int)floor(pos.y);
  int Z = (int)floor(pos.z);
  // Get relative xyz coordinates of point within that cell
  pos.x = pos.x - X;
  pos.y = pos.y - Y;
  pos.z = pos.z - Z;
  // Wrap the integer cells at 255 (smaller integer period can be introduced here)
  X = X & 255;
  Y = Y & 255;
  Z = Z & 255;

  // Calculate noise contributions from each of the eight corners
  float n000 = gradP[X + perm[Y + perm[Z]]].dot3(pos.x, pos.y, pos.z);
  float n001 = gradP[X + perm[Y + perm[Z + 1]]].dot3(pos.x, pos.y, pos.z - 1);
  float n010 = gradP[X + perm[Y + 1 + perm[Z]]].dot3(pos.x, pos.y - 1, pos.z);
  float n011 = gradP[X + perm[Y + 1 + perm[Z + 1]]].dot3(pos.x, pos.y - 1, pos.z - 1);
  float n100 = gradP[X + 1 + perm[Y + perm[Z]]].dot3(pos.x - 1, pos.y, pos.z);
  float n101 = gradP[X + 1 + perm[Y + perm[Z + 1]]].dot3(pos.x - 1, pos.y, pos.z - 1);
  float n110 = gradP[X + 1 + perm[Y + 1 + perm[Z]]].dot3(pos.x - 1, pos.y - 1, pos.z);
  float n111 = gradP[X + 1 + perm[Y + 1 + perm[Z + 1]]].dot3(pos.x - 1, pos.y - 1, pos.z - 1);

  // Compute the fade curve value for x, y, z
  float u = fade(pos.x);
  float v = fade(pos.y);
  float w = fade(pos.z);

  // Interpolate
  return lerp(
      lerp(
          lerp(n000, n100, u),
          lerp(n001, n101, u), w),
      lerp(
          lerp(n010, n110, u),
          lerp(n011, n111, u), w),
      v);
}

float SimplexNoise::simplex2(vec2 pos) const
{
  float n0, n1, n2; // Noise contributions from the three corners
  // Skew the input space to determine which simplex cell we're in
  float s = (pos.x + pos.y) * F2; // Hairy factor for 2D
  int i = (int)floor(pos.x + s);
  int j = (int)floor(pos.y + s);
  float t = (i + j) * G2;
  float x0 = pos.x - i + t; // The x,y distances from the cell origin, unskewed.
  float y0 = pos.y - j + t;
  // For the 2D case, the simplex shape is an equilateral triangle.
  // Determine which simplex we are in.
  int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
  if (x0 > y0)
  { // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    i1 = 1;
    j1 = 0;
  }
  else
  { // upper triangle, YX order: (0,0)->(0,1)->(1,1)
    i1 = 0;
    j1 = 1;
  }
  // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
  // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
  // c = (3-sqrt(3))/6
  float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
  float y1 = y0 - j1 + G2;
  float x2 = x0 - 1 + 2 * G2; // Offsets for last corner in (x,y) unskewed coords
  float y2 = y0 - 1 + 2 * G2;
  // Work out the hashed gradient indices of the three simplex corners
  i &= 255;
  j &= 255;
  Grad gi0 = gradP[i + perm[j]];
  Grad gi1 = gradP[i + i1 + perm[j + j1]];
  Grad gi2 = gradP[i + 1 + perm[j + 1]];
  // Calculate the contribution from the three corners
  float t0 = 0.5f - x0 * x0 - y0 * y0;
  if (t0 < 0)
  {
    n0 = 0;
  }
  else
  {
    t0 *= t0;
    n0 = t0 * t0 * gi0.dot2(x0, y0); // (x,y) of grad3 used for 2D gradient
  }
  float t1 = 0.5f - x1 * x1 - y1 * y1;
  if (t1 < 0)
  {
    n1 = 0;
  }
  else
  {
    t1 *= t1;
    n1 = t1 * t1 * gi1.dot2(x1, y1);
  }
  float t2 = 0.5f - x2 * x2 - y2 * y2;
  if (t2 < 0)
  {
    n2 = 0;
  }
  else
  {
    t2 *= t2;
    n2 = t2 * t2 * gi2.dot2(x2, y2);
  }
  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  return 70 * (n0 + n1 + n2);
}

float SimplexNoise::simplex3(vec3 pos) const
{
  float n0, n1, n2, n3; // Noise contributions from the four corners

  // Skew the input space to determine which simplex cell we're in
  float s = (pos.x + pos.y + pos.z) * F3; // Hairy factor for 2D
  int i = (int)floor(pos.x + s);
  int j = (int)floor(pos.y + s);
  int k = (int)floor(pos.z + s);

  float t = (i + j + k) * G3;
  float x0 = pos.x - i + t; // The x,y distances from the cell origin, unskewed.
  float y0 = pos.y - j + t;
  float z0 = pos.z - k + t;

  // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
  // Determine which simplex we are in.
  int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
  int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords
  if (x0 >= y0)
  {
    if (y0 >= z0)
    {
      i1 = 1;
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    }
    else if (x0 >= z0)
    {
      i1 = 1;
      j1 = 0;
      k1 = 0;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    }
    else
    {
      i1 = 0;
      j1 = 0;
      k1 = 1;
      i2 = 1;
      j2 = 0;
      k2 = 1;
    }
  }
  else
  {
    if (y0 < z0)
    {
      i1 = 0;
      j1 = 0;
      k1 = 1;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    }
    else if (x0 < z0)
    {
      i1 = 0;
      j1 = 1;
      k1 = 0;
      i2 = 0;
      j2 = 1;
      k2 = 1;
    }
    else
    {
      i1 = 0;
      j1 = 1;
      k1 = 0;
      i2 = 1;
      j2 = 1;
      k2 = 0;
    }
  }
  // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
  // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
  // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
  // c = 1/6.
  float x1 = x0 - i1 + G3; // Offsets for second corner
  float y1 = y0 - j1 + G3;
  float z1 = z0 - k1 + G3;

  float x2 = x0 - i2 + 2 * G3; // Offsets for third corner
  float y2 = y0 - j2 + 2 * G3;
  float z2 = z0 - k2 + 2 * G3;

  float x3 = x0 - 1 + 3 * G3; // Offsets for fourth corner
  float y3 = y0 - 1 + 3 * G3;
  float z3 = z0 - 1 + 3 * G3;

  // Work out the hashed gradient indices of the four simplex corners
  i &= 255;
  j &= 255;
  k &= 255;
  Grad gi0 = gradP[i + perm[j + perm[k]]];
  Grad gi1 = gradP[i + i1 + perm[j + j1 + perm[k + k1]]];
  Grad gi2 = gradP[i + i2 + perm[j + j2 + perm[k + k2]]];
  Grad gi3 = gradP[i + 1 + perm[j + 1 + perm[k + 1]]];

  // Calculate the contribution from the four corners
  float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
  if (t0 < 0)
  {
    n0 = 0;
  }
  else
  {
    t0 *= t0;
    n0 = t0 * t0 * gi0.dot3(x0, y0, z0); // (x,y) of grad3 used for 2D gradient
  }
  float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
  if (t1 < 0)
  {
    n1 = 0;
  }
  else
  {
    t1 *= t1;
    n1 = t1 * t1 * gi1.dot3(x1, y1, z1);
  }
  float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
  if (t2 < 0)
  {
    n2 = 0;
  }
  else
  {
    t2 *= t2;
    n2 = t2 * t2 * gi2.dot3(x2, y2, z2);
  }
  float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
  if (t3 < 0)
  {
    n3 = 0;
  }
  else
  {
    t3 *= t3;
    n3 = t3 * t3 * gi3.dot3(x3, y3, z3);
  }
  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  return 32 * (n0 + n1 + n2 + n3);
}

float SimplexNoise::fractal2(vec2 pos, octaves_t const& octaves) const
{
  float val = 0;
  for (auto const& octave : octaves) {
    val += simplex2(pos * octave.frequency) * octave.magnitude;
  }
  return val;
}
