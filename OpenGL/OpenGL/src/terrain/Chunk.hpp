#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <mutex>
#include <array>

#include "gl/Mesh.hpp"
#include "util/DataStore.hpp"
#include "blocks/Block.hpp"

/**
* Describes a subdivision of the terrain.
*/

class Chunk : public DataStore<Block::unique_ptr_t, 3> {
public:
  Chunk(glm::ivec3 chunkPos, int chunkSize);
  ~Chunk();

  Chunk(Chunk const&) = delete;
  Chunk& operator=(Chunk const&) = delete;

  /**
  * Gets the block at the given position.
  * The coordinate origin is in chunk space i.e. (0,0,0) is the chunk corner.
  * /!\ nothing guaranties the Block's longevity (hence the pointer).
  */
  Block* getBlock(glm::ivec3 pos);

  /**
  * Sets the block at the given position.
  * The coordinate origin is in chunk space i.e. (0,0,0) is the chunk corner.
  * This will update the mesh and, if possible, neighboring meshes as well.
  */
  void setBlock(glm::ivec3 pos, Block::unique_ptr_t block);

  /**
  * Sets the block at the given position, transmits to neighbors if needed.
  * The coordinate origin is in chunk space i.e. (0,0,0) is the chunk corner.
  * This will update the mesh and, if possible, neighboring meshes as well.
  */
  void setBlockAccrossChunks(glm::ivec3 pos, Block::unique_ptr_t block);

  /**
  * Signals the chunk that it must be recomputed on next update.
  * The chunk will be recomputed only if it was loaded (see isLoaded).
  */
  void markToRecompute();

  /**
  * If the chunk has been computed at least once.
  */
  bool isLoaded();

  /**
  * (re)calculates the mesh data from the blocks.
  * Thread safe.
  */
  void compute();

  /**
  * New's the Chunk if computed. Must be called before draws,
  * only in the main thread.
  */
  void update();

  /**
  * Gl draw the solid block mesh.
  * It will avoid draw calls for empty chunks.
  */
  void drawSolid();

  /**
  * Gl draw the transparent block mesh.
  * Avoids draw calls for empty chunks.
  * it will draw back to front in the order given by dir, i.e. if dir.x > 0,
  * draws the left-most quad first and the right-most quad last.
  */
  void drawTransparent(glm::vec3 dir);

  /**
  * Tells whether the chunks contains solid faces.
  */
  bool hasSolidData() const;

  /**
  * Tells whether the chunks contains transparent faces.
  */
  bool hasTransparentData() const;

  /**
  * Tells whether the chunks contains transparent or solid faces.
  */
  bool hasData() const;

  /// Some lookups for efficient code execution ///

  /**
  * Gets the neighbor at a given offset chunk position.
  * off must be between (-1,-1,-1) and (1, 1, 1) and different from (0, 0, 0).
  */
  std::weak_ptr<Chunk> getNeighbor(glm::ivec3 off);

  /**
  * The chunk's neighbors are the 26 chunks adjacent.
  * Order: see neighborOffsets
  */
  std::array<std::weak_ptr<Chunk>, 26> neighbors;

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

private:
  Block* getBlockAccrossChunks(glm::ivec3 pos);

  // the gl mesh and corresponding data.
  std::unique_ptr<Mesh> mesh;
  GLuint solidOffset = 0;
  glm::uvec3 transpOffset = glm::uvec3(0);

  MeshData meshData;
  bool loaded;
  bool computed;
  bool mustRecompute;
  std::mutex computeMutex;
  glm::mat4 model;
};
