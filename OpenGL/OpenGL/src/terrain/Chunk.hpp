#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "BlockGeom.hpp"
#include "../gl/Mesh.hpp"
#include "../util/DataStore.hpp"
#include "../blocks/Block.h"

/**
 * Describes a subdivision of the terrain.
 */

class Blocks : public DataStore<Block::unique_ptr_t, 3> {
public:
  Blocks(int size)
      : DataStore<Block::unique_ptr_t, 3>(glm::ivec3(size))
  {
  }
};


/**
 * A Chunk holds a portion of the terrain data and manages a mesh accordingly.
 */

class Chunk {
public:
  Chunk(glm::ivec3 chunkPos, Blocks blocks);
  ~Chunk();

  Chunk(Chunk const&) = delete;
  Chunk& operator=(Chunk const&) = delete;

  Mesh const& getMesh();
  Block* getBlock(glm::ivec3 pos);
  void setBlock(glm::ivec3 pos, Block::unique_ptr_t block);

private:
  void generateMeshData();
  void updateMesh();
  std::array<GLfloat, 4> genOcclusion(glm::ivec3 pos, BlockFace face);
  bool isSolid(glm::ivec3 pos);

  glm::ivec3 chunkPos;
  Blocks blocks;

  // the gl mesh and corresponding data.
  Mesh* mesh;
  std::vector<GLuint> indices;
  std::vector<GLfloat> positions, normals, textureCoords, occlusion;
};
