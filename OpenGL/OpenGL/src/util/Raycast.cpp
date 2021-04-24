#include "Raycast.hpp"
#include "debug/Debug.hpp"

using glm::vec3;
using glm::ivec3;

Raycast::Raycast(float maxDist) : maxDist(maxDist)
{ }

// credit: http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
ivec3 Raycast::cast(vec3 pos, vec3 direction, Terrain& terrain) const {

  vec3 ipos = floor(pos); // (X/Y)
  const vec3 step = sign(direction); // (stepX/stepY)
  direction = glm::normalize(direction);
  const vec3 tdelta = abs(1.f / direction); // (tDeltaX/tDeltaY)
  const vec3 offset = (ipos - pos) * step + (1.f - step) / 2.f;
  vec3 tmax = offset * tdelta; // (tMaxX/tMaxY)

  Block* block;
  float t = 0;

  while(t < maxDist) {

    // check current block
    block = terrain.getBlock(ipos);
    if(block && block->type != BlockType::Air) {
      return ipos;
    }

    // update position
    if(tmax.x < tmax.y) {
      if(tmax.x < tmax.z) {
        t = tmax.x;
        tmax.x += tdelta.x;
        ipos.x += step.x;
      } else {
        t = tmax.z;
        tmax.z += tdelta.z;
        ipos.z += step.z;
      }
    } else {
      if(tmax.y < tmax.z) {
        t = tmax.y;
        tmax.y += tdelta.y;
        ipos.y += step.y;
      } else {
        t = tmax.z;
        tmax.z += tdelta.z;
        ipos.z += step.z;
      }
    }

  }

  return ipos;
}
