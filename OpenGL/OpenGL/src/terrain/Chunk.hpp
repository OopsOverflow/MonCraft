#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <mutex>
#include <array>

#include "gl/Mesh.hpp"
#include "util/DataStore.hpp"
#include "blocks/Block.h"

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
  *
  */
  bool isLoaded();

  /**
  * (re)calculates the mesh data from the blocks.
  * Thread safe.
  */
  void compute();

  /**
  * Gl draw the mesh.
  * It will avoid draw calls for empty chunks.
  */
  void draw();


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
  static const std::array<int, 26> neighborOffsetsInverse;

private:
  void update(); // new's the Chunk if computed. Called in draw() for now.
  bool isSolid(glm::ivec3 pos);
  bool isSolidNoChecks(glm::ivec3 pos);

  glm::ivec3 chunkPos; // chunk index, not world position (position is size * chunkPos)

  // the gl mesh and corresponding data.
  std::unique_ptr<Mesh> mesh;
  std::vector<GLuint> indices;
  std::vector<GLfloat> positions, normals, textureCoords, occlusion;

  std::mutex computeMutex;
  bool computed;
};
