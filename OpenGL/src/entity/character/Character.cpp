#include "Character.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdlib.h>
#include <cmath>
#include <memory>
#include <utility>

#include "blocks/Air_Block.hpp"
#include "blocks/AllBlocks.hpp"
#include "blocks/Birch_Planks_Block.hpp"
#include "blocks/Birch_Stair_Block.hpp"
#include "blocks/Oak_Planks_Block.hpp"
#include "blocks/Oak_Stair_Block.hpp"
#include "entity/Entity.hpp"
#include "entity/Hitbox.hpp"
#include "entity/character/CharacterHitbox.hpp"
#include "entity/character/Chest.hpp"
#include "entity/character/Head.hpp"
#include "entity/character/LeftArm.hpp"
#include "entity/character/LeftLeg.hpp"
#include "entity/character/RightArm.hpp"
#include "entity/character/RightLeg.hpp"
#include "gl/Camera.hpp"
#include "terrain/World.hpp"

using namespace glm;

const float defaultSpeed = 4.317f;
const float godMultiplier = 5;
const float sprintMultiplier = 2;

Character::Character(vec3 pos)
    : Entity(CharacterHitbox()),
      view(CharacterView::FIRST_PERSON),
      caster(100), // distance the player can place blocks
      currentBlock(BlockType::Oak_Stair),
      god(true), sprint(false), dab(false)
{
  bodyNode.loc = pos;
  rootNode.sca = vec3(1.85f / 32.f); // steve is 1.85 blocks high, 32 pixels high
  rootNode.rot.y = glm::pi<float>();
  rootNode.loc.y = 9.5 / 32. * 1.85;
  headNode.loc = {0, 6, 0};
  bodyNode.addChild(&rootNode);
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


void Character::cameraToHead(Camera& camera) {
  vec3 eyePos;
  vec3 eyeTarget;
	if(view == CharacterView::FIRST_PERSON) {
		eyePos = headNode.model * vec4(0, 4, 0, 1);
		eyeTarget = headNode.model * vec4(0, 4, 50, 1);

	}
	else{
    if(view == CharacterView::THIRD_PERSON){
	  eyePos = headNode.model * vec4(0, 4, 1, 1);
    eyeTarget = headNode.model * vec4(0, 4, 0, 1);

    }
    else if(view == CharacterView::FRONT) {
      eyePos = headNode.model * vec4(0, 4, -1, 1);
      eyeTarget = headNode.model * vec4(0, 4, 0, 1);

    }
    
    camera.setLookAt(eyeTarget, eyePos);
    auto tmp = camera.getBoxCorners();
    
    float min = 4.f;
    for(size_t i = 0; i < 4; i += 1) {
      auto cast = caster.cast(tmp.at(i), eyeTarget - eyePos);

      min = std::min(min, cast.dist);
    }
    if(min > 0.5f) 
      eyePos = eyeTarget + min * normalize(eyeTarget - eyePos);
    else
      eyePos = eyeTarget + 0.5f * normalize(eyeTarget - eyePos);
    
   
  } 

  camera.setLookAt(eyePos, eyeTarget);
}

void Character::enableGodMode() {
  if(god) return;
  properties.verticalFriction = 5.5f;
  gravity = 0;
  properties.maxSpeed = defaultSpeed * godMultiplier;
  if(sprint) properties.maxSpeed *= sprintMultiplier;
  properties.maxAccel = 40.f;
  god = true;
}

void Character::disableGodMode() {
  if(!god) return;
  properties.verticalFriction = 0.f;
  gravity = 32.f;
  properties.maxSpeed = defaultSpeed;
  if(sprint) properties.maxSpeed *= sprintMultiplier;
  properties.maxAccel = 10.f;
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
    properties.maxSpeed *= sprintMultiplier;
  }
  else {
    properties.maxSpeed /= sprintMultiplier;
  }
}

void Character::setDab(bool dab) {
  this->dab = dab;
}

bool Character::getDab() const {
  return dab;
}

void Character::breakBlock() {
  if(!dab) { //we can't break when we are dabbing wtf???
    r_arm.anim->setAnimation(Animation::Break);
    
    auto& world = World::getInst();
    vec3 eyePos = headNode.model * vec4(0, 4, 0, 1);
    vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
    auto cast = caster.cast(eyePos, eyeTarget - eyePos);
    if (cast.success) {
        BlockType block = cast.block->type;
        if (block != BlockType::Air && block != BlockType::Water) {
            auto airBlock = Block::create_static<Air_Block>();
            record.push(cast.blockPosition, airBlock.get());
            world.setBlock(cast.blockPosition, move(airBlock));
        }
    }
  }
}

Facing getFacing(vec3 dir) {
  if(abs(dir.x) > abs(dir.z)) {
    if(dir.x > 0) return Facing::EAST;
    else return Facing::WEST;
  } else {
    if(dir.z > 0) return Facing::SOUTH;
    else return Facing::NORTH;
  }
}

void Character::placeBlock() {
  auto& world = World::getInst();
  vec3 eyePos = headNode.model * vec4(0, 4, 4, 1);
  vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
  auto cast = caster.cast(eyePos, eyeTarget - eyePos);

  if(cast.success) {
    if(hitbox.collides(bodyNode.loc, cast.position + cast.normal)) return;
    Block* block = world.getBlock(cast.position + cast.normal);
    if(!block) return;
    if(block->type != BlockType::Air && block->type != BlockType::Water) return;
    ivec3 pos = cast.blockPosition + cast.normal;

    Block::unique_ptr_t newBlock;
    Facing facing = getFacing(eyeTarget - eyePos);

    if(currentBlock == BlockType::Oak_Stair)
      newBlock = Block::create_dynamic<Oak_Stair_Block>(facing);
    else if(currentBlock == BlockType::Oak_Planks)
      newBlock = Block::create_dynamic<Oak_Planks_Block>(facing);
    else if(currentBlock == BlockType::Birch_Stair)
      newBlock = Block::create_dynamic<Birch_Stair_Block>(facing);
    else if(currentBlock == BlockType::Birch_Planks)
      newBlock = Block::create_dynamic<Birch_Planks_Block>(facing);
    else
      newBlock = AllBlocks::create_static(currentBlock);

    record.push(pos, newBlock.get());
    world.setBlock(pos, std::move(newBlock));
  }
}

void Character::pickBlock() {
  vec3 eyePos = headNode.model * vec4(0, 4, 0, 1);
  vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
  auto cast = caster.cast(eyePos, eyeTarget - eyePos);
  if(cast.success) currentBlock = cast.block->type;
}

void Character::update(uint32_t dt) {
  Entity::update(dt);

  // smooth head rot with constant speed
  {
    float speed;
    glm::vec3 dist;
    glm::highp_dvec3 target;
    if(!dab) {
      target = headNode.rot;
      speed = 5;
    }
    else {
      target = radians(highp_dvec3(35., -40., 0.));
      speed = 15;
    }
      
    
    dist = vec3(target - head.node.rot);
    if(dist != vec3(0)) {
      auto delta = normalize(dist) * (speed * dt * 0.001f);
      if(any(greaterThan(abs(delta), abs(dist)))) {
        head.node.rot = target;
      }
      else {
        head.node.rot += delta;
      }
    }
  }

  r_arm.animate(dt);
  l_arm.animate(dt);
  l_leg.animate(dt);
  r_leg.animate(dt);

  // // walk animation
    if(state == State::Walking) {
        r_arm.anim->setAnimation(Animation::Walk);
        l_arm.anim->setAnimation(Animation::Walk);
        r_leg.anim->setAnimation(Animation::Walk);
        l_leg.anim->setAnimation(Animation::Walk);
    }
    else {
      r_arm.anim->setAnimation(Animation::Idle);
      l_arm.anim->setAnimation(Animation::Idle);
      r_leg.anim->setAnimation(Animation::Idle);
      l_leg.anim->setAnimation(Animation::Idle);

    }

    if(dab) {
      r_arm.anim->setAnimation(Animation::Dab);
      l_arm.anim->setAnimation(Animation::Dab);
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

BlockArray& Character::getRecord() {
  return record;
}

BlockType Character::getCurrentBlock() const {
  return currentBlock;
}

void Character::setCurrentBlock(BlockType type) {
  currentBlock = type;
}
