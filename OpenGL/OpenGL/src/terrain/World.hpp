#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include "ChunkMap.hpp"
#include "gl/Camera.hpp"
#include "entity/Entities.hpp"

class Entity;

/**
* Manages the chunks, load/unload, render and manipulation of blocks.
*/

class World
{
public:
  static World& getInst();

  /**
  * Renders the visible chunks.
  */
  void render(Camera const& camera);

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

private:
  World();
  ~World();
};
