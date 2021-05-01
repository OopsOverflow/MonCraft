#include "Raycast.hpp"
#include "../debug/Debug.hpp"

using glm::vec3;
using glm::ivec3;

Raycast::Raycast(float maxDist) : maxDist(maxDist)
{ }

// credit: http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
Raycast::CastResult Raycast::cast(vec3 pos, vec3 direction, Terrain& terrain) const {

  vec3 ipos = floor(pos); // (X/Y)
  const vec3 step = sign(direction); // (stepX/stepY)
  direction = glm::normalize(direction);
  const vec3 tdelta = abs(1.f / direction); // (tDeltaX/tDeltaY)
  const vec3 offset = (ipos - pos) * step + (1.f + step) / 2.f; // <=> (step > 0) ? (1 - pos + ipos) : (pos - ipos)
  vec3 tmax = offset * tdelta; // (tMaxX/tMaxY)

  vec3 norm(0); // block face normal
  Block* block;
  float t = 0;

  while(t < maxDist) {

    // check current block
    block = terrain.getBlock(ipos);
    if(block && block->type != BlockType::Air) {
      return {true, ipos, norm};
    }

    // update position
    if(tmax.x < tmax.y) {
      if(tmax.x < tmax.z) {
        t = tmax.x;
        tmax.x += tdelta.x;
        ipos.x += step.x;
        norm = { -step.x, 0, 0 };
      } else {
        t = tmax.z;
        tmax.z += tdelta.z;
        ipos.z += step.z;
        norm = { 0, 0, -step.z };
      }
    } else {
      if(tmax.y < tmax.z) {
        t = tmax.y;
        tmax.y += tdelta.y;
        ipos.y += step.y;
        norm = { 0, -step.y, 0 };
      } else {
        t = tmax.z;
        tmax.z += tdelta.z;
        ipos.z += step.z;
        norm = { 0, 0, -step.z };
      }
    }

  }

  return {false, ipos, norm};
}
