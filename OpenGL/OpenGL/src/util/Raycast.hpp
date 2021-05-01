#pragma once

#include "../terrain/Terrain.hpp"

class Raycast {
public:
  Raycast(float maxDist);

  struct CastResult {
    bool success; // false if max dist was reached
    glm::vec3 position; // found block position
    glm::vec3 normal; // which block face (normal) was hit first by the ray
  };

  // credit: http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
  CastResult cast(glm::vec3 pos, glm::vec3 direction, Terrain& terrain) const;

  float maxDist;
};
