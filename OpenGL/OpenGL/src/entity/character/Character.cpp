#include "Character.hpp"
#include "CharacterHitbox.hpp"
#include "blocks/Debug_Block.hpp"
#include "blocks/Air_Block.hpp"
#include "blocks/Dirt_Block.hpp"
#include "blocks/Tallgrass_Block.hpp"
#include "blocks/Water_Block.hpp"
#include "blocks/AllBlocks.hpp"
#include "gl/ResourceManager.hpp"
#include "terrain/World.hpp"

using namespace glm;

const float defaultSpeed = 4.317f;
const float godMultiplier = 5;
const float sprintMultiplier = 2;

Character::Character(vec3 pos)
    : Entity(CharacterHitbox()),
      caster(100), // distance the player can place blocks
      currentBlock(BlockType::Dirt),
      god(true), sprint(false)
{
  node.loc = pos;
  rootNode.sca = vec3(1.85f / 32.f); // steve is 1.85 blocks high, 32 pixels high
  rootNode.rot.y = glm::pi<float>();
  rootNode.loc.y = 9.5 / 32. * 1.85;
  headNode.loc = {0, 6, 0};
  node.addChild(&rootNode);
  rootNode.addChild(&headNode);
  rootNode.addChild(&chest.node);
  rootNode.addChild(&head.node);
  chest.node.addChild(&l_arm.node);
  chest.node.addChild(&r_arm.node);
  chest.node.addChild(&l_leg.node);
  chest.node.addChild(&r_leg.node);
  animState = 0;

  god = !god;
  if(!god) enableGodMode();
  else disableGodMode();
}

Character::~Character() {}

#include "debug/Debug.hpp"


void Character::enableGodMode() {
  if(god) return;
  verticalFriction = 5.5f;
  gravity = 0;
  maxSpeed = defaultSpeed * godMultiplier;
  if(sprint) maxSpeed *= sprintMultiplier;
  maxAccel = 40.f;
  god = true;
}

void Character::disableGodMode() {
  if(!god) return;
  verticalFriction = 0.f;
  gravity = 32.f;
  maxSpeed = defaultSpeed;
  if(sprint) maxSpeed *= sprintMultiplier;
  maxAccel = 10.f;
  god = false;
}

void Character::toggleGodMode() {
  if(god) disableGodMode();
  else enableGodMode();
}

bool Character::getGodMode() {
  return god;
}

void Character::setSprint(bool sprint) {
  if(this->sprint == sprint) return;
  this->sprint = sprint;

  if(sprint) {
    maxSpeed *= sprintMultiplier;
  }
  else {
    maxSpeed /= sprintMultiplier;
  }
}


void Character::breakBlock() {
  vec3 eyePos = headNode.model * vec4(0, 4, 0, 1);
  vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
  auto cast = caster.cast(eyePos + .5f, eyeTarget - eyePos);
  if (cast.success) {
      BlockType block = cast.block->type;
      if (block != BlockType::Air && block != BlockType::Water) {
          World::getInst().setBlock(cast.position, Block::create_static<Air_Block>());
          record.push_back({ cast.position, BlockType::Air });
      }
  }
}

void Character::placeBlock() {
  auto& world = World::getInst();
  vec3 eyePos = headNode.model * vec4(0, 4, 4, 1);
  vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
  auto cast = caster.cast(eyePos + .5f, eyeTarget - eyePos);

  if(cast.success) {
    if(hitbox.collides(node.loc, cast.position + cast.normal)) return;
    Block* block = world.getBlock(cast.position + cast.normal);
    if(!block) return;
    if(block->type != BlockType::Air && block->type != BlockType::Water) return;
    ivec3 pos = cast.position + cast.normal;
    world.setBlock(pos, AllBlocks::create_static(currentBlock));
    record.push_back({ pos, currentBlock });
  }
}

void Character::pickBlock() {
  vec3 eyePos = headNode.model * vec4(0, 4, 0, 1);
  vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
  auto cast = caster.cast(eyePos + .5f, eyeTarget - eyePos);
  if(cast.success) currentBlock = cast.block->type;
}

void Character::update(float dt) {
  Entity::update(dt);

  // smooth head rot with constant speed
  {
    float speed = 5;
    auto dist = vec3(headNode.rot - head.node.rot);
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
  chest.draw();
  head.draw();
  l_arm.draw();
  r_arm.draw();
  l_leg.draw();
  r_leg.draw();
}

BlockArray Character::getRecord() {
  BlockArray res = record;
  record.clear();
  return res;
}
