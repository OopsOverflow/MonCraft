#pragma once

#include "../terrain/Terrain.hpp"

class Raycast {
public:
  Raycast(float maxDist);

  // credit: http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
  glm::ivec3 cast(glm::vec3 pos, glm::vec3 direction, Terrain& terrain) const;

  float maxDist;
};
