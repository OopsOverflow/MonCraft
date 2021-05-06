#include "Raycast.hpp"
#include "../debug/Debug.hpp"

using glm::vec3;
using glm::ivec3;

Raycast::Raycast(float maxDist) : maxDist(maxDist)
{ }

// credit: http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
Raycast::CastResult Raycast::cast(vec3 pos, vec3 direction, Terrain& terrain) const {

  CastResult res{
    false,
    floor(pos),
    vec3(0),
    nullptr
  };

  const vec3 step = sign(direction); // (stepX/stepY)
  direction = glm::normalize(direction);
  const vec3 tdelta = abs(1.f / direction); // (tDeltaX/tDeltaY)
  const vec3 offset = (res.position - pos) * step + (1.f + step) / 2.f; // <=> (step > 0) ? (1 - pos + ipos) : (pos - ipos)
  vec3 tmax = offset * tdelta; // (tMaxX/tMaxY)

  float t = 0;

  while(t < maxDist) {

    // check current block
    res.block = terrain.getBlock(res.position);
    if(res.block && res.block->type != BlockType::Air) {
      res.success = true;
      return res;
    }

    // update position
    if(tmax.x < tmax.y) {
      if(tmax.x < tmax.z) {
        t = tmax.x;
        tmax.x += tdelta.x;
        res.position.x += step.x;
        res.normal = { -step.x, 0, 0 };
      } else {
        t = tmax.z;
        tmax.z += tdelta.z;
        res.position.z += step.z;
        res.normal = { 0, 0, -step.z };
      }
    } else {
      if(tmax.y < tmax.z) {
        t = tmax.y;
        tmax.y += tdelta.y;
        res.position.y += step.y;
        res.normal = { 0, -step.y, 0 };
      } else {
        t = tmax.z;
        tmax.z += tdelta.z;
        res.position.z += step.z;
        res.normal = { 0, 0, -step.z };
      }
    }

  }

  return res;
}
