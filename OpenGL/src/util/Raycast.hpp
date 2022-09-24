#pragma once

#include <glm/glm.hpp>

class Block;
class Entity;
/**
 * Raycast checks for solid blocks along a ray.
 */

class Raycast {
public:

  /**
   * Constructs a ray with a maximum cast distance
   */
  Raycast();

  struct BlockCastResult {
    bool success;       // false if max dist was reached
    glm::vec3 position; // found ray position
    glm::vec3 normal;   // which block face (normal) was hit first by the ray
    glm::vec3 blockPosition; //position of the block hitted by the ray
    Block* block;       // the hit target. Undefined content if success is false
    float dist;
  };

  struct EntityCastResult {
    bool success;       // false if max dist was reached
    glm::vec3 position; // found ray position
    Entity* entity;     // the hit target. Undefined content if success is false
    float dist;
  };

  /**
   * Performs a ray cast on blocks.
   * credit: http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
   */
  BlockCastResult blockCast(glm::vec3 pos, glm::vec3 direction, float maxDist) const;

  /**
   * Performs a ray cast on entities.
   * credit: https://www.researchgate.net/publication/220494140_An_Efficient_and_Robust_Ray-Box_Intersection_Algorithm
   */
  EntityCastResult entityCast(glm::vec3 pos, glm::vec3 direction, float maxDist) const;
};
