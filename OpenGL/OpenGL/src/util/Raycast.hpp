#pragma once

#include "../terrain/Terrain.hpp"

/**
 * Raycast checks for solid blocks along a ray.
 */

class Raycast {
public:

  /**
   * Constructs a ray with a maximum cast distance
   */
  Raycast(float maxDist);

  struct CastResult {
    bool success;       // false if max dist was reached
    glm::vec3 position; // found block position
    glm::vec3 normal;   // which block face (normal) was hit first by the ray
    Block* block;       // the hit target. Undefined content if success is false
  };

  /**
   * Performs a ray cast.
   * credit: http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
   */
  CastResult cast(glm::vec3 pos, glm::vec3 direction, Terrain& terrain) const;

  float maxDist;
};
