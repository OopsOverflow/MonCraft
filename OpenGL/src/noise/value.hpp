#pragma once

#include <glm/glm.hpp>
#include <stdint.h>

/**
 * A non-iterative 2D-space value noise generator.
 * Generates reproducible noise based on 2D coordinates and a seed.
 */

class ValueNoise {
public:
  /**
   * Constructs a value noise generator.
   * samples are reproducible.
   */
  ValueNoise(uint32_t seed);

  /**
   * Samples a point in 2D space and returns a pseudo-random
   * linearly distributed integer in the range [0, INT_MAX]
   */
  uint32_t sample1D(const glm::ivec2 &pos) const;

  /**
   * Samples a point in 2D space and returns a pair of pseudo-random
   * linearly distributed integers in the range [0, UINT16_MAX]
   */
  glm::vec<2, uint16_t> sample2D(const glm::ivec2 &pos) const;

  /**
   * Samples a point in 2D space and returns a triplet of pseudo-random
   * linearly distributed integers in the range [0, UINT8_MAX]
   */
  glm::vec<3, uint8_t> sample3D(const glm::ivec2 &pos) const;

  // /**
  //  * Convenience method to generate a bitmap out of the voronoi noise.
  //  */
  // void generate(Bitmap &map);

private:
  uint32_t seed;
  uint32_t rand(uint64_t prn) const;
};
