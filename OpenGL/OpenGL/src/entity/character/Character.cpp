#include "Character.hpp"
#include "blocks/Debug_Block.h"
#include "blocks/Air_Block.h"
#include "blocks/Dirt_Block.h"

using namespace glm;

Character::Character(vec3 pos)
    : caster(100) // distance the player can place blocks
{
  node.loc = pos;
  rootNode.sca = vec3(1.85 / 32.f); // steve is 1.85 blocks high, 32 pixels high
  rootNode.rot.y = glm::pi<float>();
  headNode.loc = {0, 6, 0};
  node.addChild(&rootNode);
  rootNode.addChild(&headNode);
  rootNode.addChild(&chest.node);
  rootNode.addChild(&head.node);
  chest.node.addChild(&l_arm.node);
  chest.node.addChild(&r_arm.node);
  chest.node.addChild(&l_leg.node);
  chest.node.addChild(&r_leg.node);

  tex = texLoader.loadTexture("Character");
  animState = 0;
}

Character::~Character() {}

#include "debug/Debug.hpp"

void Character::breakBlock(Terrain& terrain) {
  vec3 eyePos = headNode.model * vec4(0, 4, 4, 1);
  vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
  auto cast = caster.cast(eyePos + .5f, eyeTarget - eyePos, terrain);

  if(cast.success && cast.block->type != BlockType::Air) {
    terrain.setBlock(cast.position, Block::create_static<Air_Block>());
  }
}

void Character::placeBlock(Terrain& terrain) {
  vec3 eyePos = headNode.model * vec4(0, 4, 4, 1);
  vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
  auto cast = caster.cast(eyePos + .5f, eyeTarget - eyePos, terrain);

  if(cast.success && terrain.getBlock(cast.position + cast.normal)->type == BlockType::Air) { // TODO: will crash in chunk is unloaded
    terrain.setBlock(cast.position + cast.normal, Block::create_static<Dirt_Block>());
  }
}

void Character::update(float dt) {
  Entity::update(dt);

  // smooth head rot with constant speed
  {
    float speed = 5;
    auto dist = headNode.rot - head.node.rot;
    if(dist != vec3(0)) {
      auto delta = normalize(dist) * speed * dt;
      if(any(greaterThan(abs(delta), abs(dist)))) {
        head.node.rot = headNode.rot;
      }
      else {
        head.node.rot += delta;
      }
    }
  }

  // walk animation
  if(state == State::Walking) {
    float speed = 10;
    float dx = cos(speed * animState);

    l_leg.node.rot.x = dx;
    r_leg.node.rot.x = -dx;
    l_arm.node.rot.x = -dx;
    r_arm.node.rot.x = dx;

    animState += dt;
  }

  // transition to stand-by
  else {
    float speed = 10;
    auto dist = l_leg.node.rot.x;
    if(dist != 0) {
      auto delta = dist * speed * dt;
      if(abs(delta) > abs(dist)) {
        l_leg.node.rot.x = 0;
        r_leg.node.rot.x = 0;
        l_arm.node.rot.x = 0;
        r_arm.node.rot.x = 0;
        animState = 0;
      }
      else {
        l_leg.node.rot.x -= delta;
        r_leg.node.rot.x += delta;
        l_arm.node.rot.x += delta;
        r_arm.node.rot.x -= delta;
      }
    }
  }

}

void Character::render() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  chest.draw();
  head.draw();
  l_arm.draw();
  r_arm.draw();
  l_leg.draw();
  r_leg.draw();
  glBindTexture(GL_TEXTURE_2D, 0);
}
