#pragma once

#include "terrain/Terrain.hpp"

/**
 * Manages collisions between an entity and the terrain.
 */

class Hitbox {

public:
  /**
   * Creates a hitbox. The hitbox is a cube where:
   * c1 is the minimum corner (smallest x/y/z) and c2 is the maximum corner.
   */
  Hitbox(glm::vec3 c1, glm::vec3 c2);

  /**
   * Computes the offset the hitbox has to move in order not to go in the wall.
   * 0 means no offset of course.
   */
  glm::vec3 computeCollision(glm::vec3 pos, glm::vec3 displ, Terrain& terrain);

  /**
   * Checks if the hitbox collides with slid blocks.
   */
  bool collides(glm::vec3 pos, glm::ivec3 blockPos);

private:
  glm::vec3 c1, c2;
};
