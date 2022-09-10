#pragma once

#include <glm/glm.hpp>

class Block;
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
    glm::vec3 position; // found ray position
    glm::vec3 normal;   // which block face (normal) was hit first by the ray
    glm::vec3 blockPosition; //position of the block hitted by the ray
    Block* block;       // the hit target. Undefined content if success is false
    float dist;
  };

  /**
   * Performs a ray cast.
   * credit: http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
   */
  CastResult cast(glm::vec3 pos, glm::vec3 direction) const;

  float maxDist;
};
