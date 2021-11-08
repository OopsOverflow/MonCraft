#pragma once

#include <memory>
#include <array>
#include <glm/glm.hpp>

#include "util/DataStore.hpp"
#include "blocks/Block.hpp"

/**
* Describes a subdivision of the terrain.
*/

class AbstractChunk : public DataStore<Block::unique_ptr_t, 3> {
public:
  AbstractChunk(glm::ivec3 chunkPos, int chunkSize);
  virtual ~AbstractChunk();

  static AbstractChunk* create(glm::ivec3 chunkPos, int chunkSize);

  AbstractChunk(AbstractChunk const&) = delete;
  AbstractChunk& operator=(AbstractChunk const&) = delete;

  /**
  * Sets the block at the given position.
  * The coordinate origin is in chunk space i.e. (0,0,0) is the chunk corner.
  * This will update the mesh and, if possible, neighboring meshes as well.
  */
  virtual void setBlock(glm::ivec3 pos, Block::unique_ptr_t block);

  /**
  * (re)calculates the mesh data from the blocks.
  * Thread safe.
  */
  virtual void compute();

  /**
  * New's the Chunk if computed. Must be called before draws,
  * only in the main thread.
  */
  virtual void update();

  bool hasAllNeighbors() const;

  /// Some lookups for efficient code execution ///

  /**
  * Gets the neighbor at a given offset chunk position.
  * off must be between (-1,-1,-1) and (1, 1, 1) and different from (0, 0, 0).
  */
  std::weak_ptr<AbstractChunk> getNeighbor(glm::ivec3 off);

  /**
  * The chunk's neighbors are the 26 chunks adjacent.
  * Order: see neighborOffsets
  */
  std::array<std::weak_ptr<AbstractChunk>, 26> neighbors;

  /**
  * Specifies the offset position of a neighbor relative to the current chunk.
  * The nth neighbor occupies the position chunkPos + neighborOffsets[n]
  */
  static const std::array<glm::ivec3, 26> neighborOffsets;

  /**
  * Gets n such that neighborOffsets[n] = -neighborOffsets[neighborOffsetsInverse[n]]
  */
  static const std::array<int, 26> neighborOffsetsInverse;

  const glm::ivec3 chunkPos; // chunk index, not world position (position is size * chunkPos)

protected:
  void setBlockAccrossChunks(glm::ivec3 pos, Block::unique_ptr_t block);
  Block* getBlockAccrossChunks(glm::ivec3 pos) const;
};
