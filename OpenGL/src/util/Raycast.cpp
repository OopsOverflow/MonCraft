#include "Raycast.hpp"

#include <glm/glm.hpp>

#include "blocks/Block.hpp"
#include "terrain/World.hpp"
#include "entity/Entity.hpp"

using glm::vec3;
using glm::ivec3;

Raycast::Raycast()
{ }

// credit: http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
Raycast::BlockCastResult Raycast::blockCast(vec3 pos, vec3 direction, float maxDist) const {
  auto& world = World::getInst();

  pos += .5f;

  BlockCastResult res{
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

#include <iostream>

Raycast::EntityCastResult Raycast::entityCast(glm::vec3 pos, glm::vec3 direction, float maxDist) const {
  EntityCastResult res{
    false,
    pos,
    nullptr,
    maxDist
  };

    glm::vec3 invDir = 1.0f / direction;
    int sign[3];
    sign[0] = (invDir.x < 0);
    sign[1] = (invDir.y < 0);
    sign[2] = (invDir.z < 0);

    auto entities = World::getInst().entities.getAll();
    for(auto entity : entities) {
      glm::highp_dvec3 bounds[2];
      entity->hitbox.getBounds(bounds[0], bounds[1]);
      bounds[0] += entity->getPosition();
      bounds[1] += entity->getPosition();
      float tmin = ((float)bounds[sign[0]].x - pos.x) * invDir.x;
      float tmax = ((float)bounds[1 - sign[0]].x - pos.x) * invDir.x;
      float tymin = ((float)bounds[sign[1]].y - pos.y) * invDir.y;
      float tymax = ((float)bounds[1 - sign[1]].y - pos.y) * invDir.y;
    
      if(tmin > tymax || tymin > tmax)
        continue;
      if(tymin > tmin)
        tmin = tymin;
      if(tymax < tmax)
        tmax = tymax;

      float tzmin = ((float)bounds[sign[2]].z - pos.z) * invDir.z;
      float tzmax = ((float)bounds[1 - sign[2]].z - pos.z) * invDir.z;

      if(tmin > tzmax || tzmin > tmax)
        continue;
      if(tzmin > tmin)
        tmin = tzmin;

      if(abs(tmin) < res.dist) {
        res.dist = abs(tmin);
        res.position = pos + direction * res.dist;
        res.entity = entity.get();
        res.success = true;
      }

    }
    return res;
}