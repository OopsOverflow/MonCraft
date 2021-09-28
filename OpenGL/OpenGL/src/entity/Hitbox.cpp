#include "Hitbox.hpp"
#include "debug/Debug.hpp"
#include "terrain/World.hpp"

using namespace glm;

Hitbox::Hitbox(glm::highp_dvec3 c1, glm::highp_dvec3 c2)
 : c1(c1), c2(c2)
{}


highp_dvec3 Hitbox::computeCollision(highp_dvec3 pos, highp_dvec3 displ) {
  World& world = World::getInst();
  double thresold = 0.0001; // histeresis

  auto isSolid = [&](ivec3 ipos) {
    Block* block = world.getBlock(ipos);
    if(block && block->isSolid()) {
      return true;
    }
    return false;
  };

  auto testComponent = [&](int c) {

    highp_dvec3 p1 = pos + c1;
    highp_dvec3 p2 = pos + c2;

    ivec3 oldStart = floor(p1 + .5);
    ivec3 oldEnd = floor(p2 + .5);

    p1[c] += displ[c];
    p2[c] += displ[c];

    ivec3 start = floor(p1 + .5);
    ivec3 end = floor(p2 + .5);

    if(start[c] < oldStart[c] || end[c] > oldEnd[c]) {

      ivec3 ipos(0);
      for(ipos.x = start.x; ipos.x <= end.x; ipos.x++) {
        for(ipos.y = start.y; ipos.y <= end.y; ipos.y++) {
          for(ipos.z = start.z; ipos.z <= end.z; ipos.z++) {

            if(isSolid(ipos)) {
              if(start[c] < oldStart[c]) {
                pos[c] = ipos[c] + .5f - c1[c] + thresold;
                return;
              }
              else if(end[c] > oldEnd[c]) {
                pos[c] = ipos[c] - .5f - c2[c] - thresold;
                return;
              }
            }

          }
        }
      }

    }

    pos[c] += displ[c];
  };

  testComponent(0);
  testComponent(1);
  testComponent(2);

  return pos;
}

bool Hitbox::collides(highp_dvec3 pos, ivec3 blockPos) {
  ivec3 min = floor(pos + c1 + .5);
  ivec3 max = floor(pos + c2 + .5);

  return all(greaterThanEqual(blockPos, min)) && all(lessThanEqual(blockPos, max));
}
