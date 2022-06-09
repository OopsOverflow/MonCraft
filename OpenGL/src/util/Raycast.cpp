#include "Raycast.hpp"

#include <glm/glm.hpp>

#include "blocks/Block.hpp"
#include "terrain/World.hpp"

using glm::vec3;
using glm::ivec3;

Raycast::Raycast(float maxDist) : maxDist(maxDist)
{ }

// credit: http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
Raycast::CastResult Raycast::cast(vec3 pos, vec3 direction) const {
  auto& world = World::getInst();

  pos += .5f;

  CastResult res{
    false,
    pos,
    vec3(0),
    floor(pos),
    nullptr,
    0.f
  };

  const vec3 step = sign(direction); // (stepX/stepY)
  direction = glm::normalize(direction);
  const vec3 tdelta = abs(1.f / direction); // (tDeltaX/tDeltaY)
  const vec3 offset = (res.blockPosition - pos) * step + (1.f + step) / 2.f; // <=> (step > 0) ? (1 - pos + ipos) : (pos - ipos)
  vec3 tmax = offset * tdelta; // (tMaxX/tMaxY)
  


  while(res.dist < maxDist) {

    // check current block
    res.block = world.getBlock(res.blockPosition);
    if (res.block){
        BlockType block = res.block->type;
        if(block != BlockType::Air && block != BlockType::Water) {
        res.success = true;
        res.position = pos - 0.5f + direction * res.dist;
        return res;
      }
    }

    // update position
    if(tmax.x < tmax.y) {
      if(tmax.x < tmax.z) {
        res.dist = tmax.x;
        tmax.x += tdelta.x;
        res.blockPosition.x += step.x;
        res.normal = { -step.x, 0, 0 };
      } else {
        res.dist = tmax.z;
        tmax.z += tdelta.z;
        res.blockPosition.z += step.z;
        res.normal = { 0, 0, -step.z };
      }
    } else {
      if(tmax.y < tmax.z) {
        res.dist = tmax.y;
        tmax.y += tdelta.y;
        res.blockPosition.y += step.y;
        res.normal = { 0, -step.y, 0 };
      } else {
        res.dist = tmax.z;
        tmax.z += tdelta.z;
        res.blockPosition.z += step.z;
        res.normal = { 0, 0, -step.z };
      }
    }

  }

  res.position = pos - 0.5f + direction * res.dist;
  res.dist = maxDist;
  return res;
}
