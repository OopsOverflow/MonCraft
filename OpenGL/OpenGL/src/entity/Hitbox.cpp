#include "Hitbox.hpp"
#include "debug/Debug.hpp"

using namespace glm;

Hitbox::Hitbox(glm::vec3 c1, glm::vec3 c2)
 : c1(c1), c2(c2)
{}


glm::vec3 Hitbox::computeCollision(glm::vec3 pos, vec3 displ, Terrain& terrain) {

  float thresold = length(displ)*0.15f + 0.0001f; // histeresis

  auto isSolid = [&](ivec3 ipos) {
    Block* block = terrain.getBlock(ipos);
    if(block && block->type != BlockType::Air) {
      return true;
    }
    return false;
  };

  auto testComponent = [&](int c) {

    vec3 p1 = pos + c1;
    vec3 p2 = pos + c2;

    ivec3 oldStart = floor(p1 + .5f);
    ivec3 oldEnd = floor(p2 + .5f);

    p1[c] += displ[c];
    p2[c] += displ[c];

    ivec3 start = floor(p1 + .5f);
    ivec3 end = floor(p2 + .5f);

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

bool Hitbox::collides(vec3 pos, ivec3 blockPos) {
  ivec3 min = floor(pos + c1 + .5f);
  ivec3 max = floor(pos + c2 + .5f);

  return all(greaterThanEqual(blockPos, min)) && all(lessThanEqual(blockPos, max));
}
