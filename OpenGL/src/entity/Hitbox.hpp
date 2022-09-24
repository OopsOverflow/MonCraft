#pragma once

#include <glm/glm.hpp>

/**
 * Manages collisions between an entity and the terrain.
 */

class Hitbox {

public:
  /**
   * Creates a hitbox. The hitbox is a cube where:
   * c1 is the minimum corner (smallest x/y/z) and c2 is the maximum corner.
   */
  Hitbox(glm::highp_dvec3 c1, glm::highp_dvec3 c2);

  /**
   * Computes the offset the hitbox has to move in order not to go in the wall.
   * 0 means no offset of course.
   */
  glm::highp_dvec3 computeCollision(glm::highp_dvec3 pos, glm::highp_dvec3 displ);

  /**
   * Checks if the hitbox collides with slid blocks.
   */
  bool collides(glm::highp_dvec3 pos, glm::ivec3 blockPos);

  void getBounds(glm::highp_dvec3& c1, glm::highp_dvec3& c2) {c1 = this->c1; c2 = this->c2;}

private:
  glm::highp_dvec3 c1, c2;
};
