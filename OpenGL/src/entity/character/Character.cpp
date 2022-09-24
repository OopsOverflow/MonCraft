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
#include "terrain/World.hpp"

#include "debug/Debug.hpp"

using namespace glm;
const float sprintMultiplier = 2;

Character::Character(vec3 pos)
    : Entity(CharacterHitbox()),
      currentBlock(BlockType::Oak_Stair),
      sprint(false), dab(false)
{
  setPosition(pos);
  updateProperties();
}

Character::~Character() {}

void Character::handleAction(Action action) {
  switch (action)
  {
  case (Action::ACTION_0_DOWN):
    leftClick();
    break;
  case (Action::ACTION_1_DOWN):
    middleClick();
    break;
  case (Action::ACTION_2_DOWN):
    rightClick();
    break;
  case (Action::ACTION_3_DOWN):
    sprint = true;
    updateProperties();
    break;
  case (Action::ACTION_3_UP):
    sprint = false;
    updateProperties();
    break;
  case (Action::ACTION_4_DOWN):
    dab = true;
    break;
  case (Action::ACTION_4_UP):
    dab = false;
    break;
  default:
    break;
  }
}

void Character::leftClick() {
  if(!dab) { //we can't break when we are dabbing wtf???
    hasBreak = true;
    breaked = true;
    
    auto& world = World::getInst();
    vec3 eyePos = headNode.model * vec4(0, 4, 0, 1);
    vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
    auto blockCast = caster.blockCast(eyePos, eyeTarget - eyePos, 5.f);
    auto entityCast = caster.entityCast(eyePos, eyeTarget - eyePos, 5.f);
    if(blockCast.success && blockCast.dist < entityCast.dist) {
        BlockType block = blockCast.block->type;
        if (block != BlockType::Air && block != BlockType::Water) {
            auto airBlock = Block::create_static<Air_Block>();
            record.push(blockCast.blockPosition, airBlock.get());
            world.setBlock(blockCast.blockPosition, move(airBlock));
        }
    }
    else if(entityCast.success) {
      
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

void Character::rightClick() {
  auto& world = World::getInst();
  vec3 eyePos = headNode.model * vec4(0, 4, 4, 1);
  vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
  auto cast = caster.blockCast(eyePos, eyeTarget - eyePos, 5.f);

  if(cast.success) {
    hasBreak = true;
    breaked = true;
    if(hitbox.collides(bodyNode.loc, cast.blockPosition + cast.normal)) return;
    Block* block = world.getBlock(cast.blockPosition + cast.normal);
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

void Character::middleClick() {
  vec3 eyePos = headNode.model * vec4(0, 4, 0, 1);
  vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
  auto cast = caster.blockCast(eyePos, eyeTarget - eyePos, 5.f);
  if(cast.success) currentBlock = cast.block->type;
}

void Character::updateProperties() {
  Entity::updateProperties();
  if(sprint)
    properties.maxSpeed *= sprintMultiplier;
  
}

BlockType Character::getCurrentBlock() const {
  return currentBlock;
}

void Character::setCurrentBlock(BlockType type) {
  currentBlock = type;
}


void Character::serialize(sf::Packet& packet) {
  Entity::serialize(packet);
  packet << dab; 
}

void Character::read(sf::Packet& packet) {
  Entity::read(packet);
  packet >> dab;
}

void Character::consume(sf::Packet& packet) {
  Entity::consume(packet);
  decltype(dab) dab;
  packet >> dab;
}