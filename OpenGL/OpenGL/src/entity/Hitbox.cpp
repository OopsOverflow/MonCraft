#include "Hitbox.hpp"
#include "debug/Debug.hpp"

using namespace glm;

Hitbox::Hitbox(glm::vec3 c1, glm::vec3 c2)
 : c1(c1), c2(c2)
{}


glm::vec3 Hitbox::computeCollision(glm::vec3 pos, vec3 displ, Terrain& terrain) {

  auto isSolid = [&](ivec3 ipos) {
    Block* block = terrain.getBlock(ipos);
    if(block && block->type != BlockType::Air) {
      return true;
    }
    return false;
  };

  ivec3 ipos(0);
  vec3 res(0);

  vec3 p1 = pos + c1;
  vec3 p2 = pos + c2;

  ivec3 oldStart = floor(p1 + .5f);
  ivec3 oldEnd = floor(p2 + .5f);

  p1 += displ;
  p2 += displ;

  ivec3 start = floor(p1 + .5f);
  ivec3 end = floor(p2 + .5f);

  ivec3 dir = sign(displ);

    for(ipos.x = oldStart.x; ipos.x <= oldEnd.x; ipos.x++) {
      for(ipos.y = oldStart.y; ipos.y <= oldEnd.y; ipos.y++) {
        for(ipos.z = oldStart.z; ipos.z <= oldEnd.z; ipos.z++) {
          if(isSolid(ipos)) {
            std::cout << "Things got out of hand" << std::endl;
            // return res;
          }
        }
      }
    }

  if(oldStart.x != start.x && dir.x < 0) {
    ipos.x = start.x;
    for(ipos.y = oldStart.y; ipos.y <= oldEnd.y; ipos.y++) {
      for(ipos.z = oldStart.z; ipos.z <= oldEnd.z; ipos.z++) {
        if(isSolid(ipos)) res.x = ipos.x + .5f - p1.x;
      }
    }
  }

  if(oldStart.y != start.y && dir.y < 0) {
    ipos.y = start.y;
    for(ipos.x = oldStart.x; ipos.x <= oldEnd.x; ipos.x++) {
      for(ipos.z = oldStart.z; ipos.z <= oldEnd.z; ipos.z++) {
        if(isSolid(ipos)) res.y = ipos.y + .5f - p1.y;
      }
    }
  }

  if(oldStart.z != start.z && dir.z < 0) {
    ipos.z = start.z;
    for(ipos.x = oldStart.x; ipos.x <= oldEnd.x; ipos.x++) {
      for(ipos.y = oldStart.y; ipos.y <= oldEnd.y; ipos.y++) {
        if(isSolid(ipos)) res.z = ipos.z + .5f - p1.z;
      }
    }
  }

  if(oldEnd.x != end.x && dir.x > 0) {
    ipos.x = end.x;
    for(ipos.y = oldStart.y; ipos.y <= oldEnd.y; ipos.y++) {
      for(ipos.z = oldStart.z; ipos.z <= oldEnd.z; ipos.z++) {
        if(isSolid(ipos)) res.x = -displ.x;
      }
    }
  }

  if(oldEnd.y != end.y && dir.y > 0) {
    ipos.y = end.y;
    for(ipos.x = oldStart.x; ipos.x <= oldEnd.x; ipos.x++) {
      for(ipos.z = oldStart.z; ipos.z <= oldEnd.z; ipos.z++) {
        if(isSolid(ipos)) res.y = -displ.y;
      }
    }
  }

  if(oldEnd.z != end.z && dir.z > 0) {
    ipos.z = end.z;
    for(ipos.x = oldStart.x; ipos.x <= oldEnd.x; ipos.x++) {
      for(ipos.y = oldStart.y; ipos.y <= oldEnd.y; ipos.y++) {
        if(isSolid(ipos)) {
          res.z = -displ.z;
        }
      }
    }
  }

  // std::cout << "--" << std::endl;

  return res;
}
