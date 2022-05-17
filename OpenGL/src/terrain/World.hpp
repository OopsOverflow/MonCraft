#pragma once

#include <glm/glm.hpp>

#include "terrain/ChunkMap.hpp"
#include "entity/Entities.hpp"
#include "blocks/Block.hpp"
#include "terrain/ChunkImpl.hpp"

/**
* Manages the chunks, load/unload, render and manipulation of blocks.
*/

class World
{
public:
  static World& getInst();

  /**
  * Gets a block in the world.
  * if block is unavailable, returns nullptr.
  */
  Block* getBlock(glm::ivec3 pos);

  /**
  * Sets a block in the world.
  * return false if the block is unavailable.
  */
  bool setBlock(glm::ivec3 pos, Block::unique_ptr_t block);

  static const int chunkSize = 16;

  ChunkMap chunks;
  Entities entities;
  float t;
  float dt;

private:
  World();
  ~World();
};
