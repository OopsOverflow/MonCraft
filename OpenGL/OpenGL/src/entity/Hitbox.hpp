#pragma once

#include "terrain/Terrain.hpp"

class Hitbox {

public:
  Hitbox(glm::vec3 c1, glm::vec3 c2);

  /**
   * Computes the offset the hitbox has to move in order not to go in the wall.
   * 0 means no offset of course.
   */
  glm::vec3 computeCollision(glm::vec3 pos, glm::vec3 displ, Terrain& terrain);

private:
  glm::vec3 c1, c2;
};
