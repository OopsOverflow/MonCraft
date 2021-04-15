#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "BlockGeom.hpp"
#include "../util/Mesh.hpp"
#include "../util/DataStore.hpp"
#include "../blocks/Block.h"

/**
 * Describes a subdivision of the terrain.
 */

class Blocks : public DataStore<std::unique_ptr<Block>, 3> {
public:
  Blocks(int size)
      : DataStore<std::unique_ptr<Block>, 3>(glm::ivec3(size))
  { }
};


/**
 * A Chunk holds a portion of the terrain data and manages a mesh accordingly.
 */

class Chunk {
public:
  Chunk(glm::ivec2 chunkPos, Blocks blocks);
  ~Chunk();

  Chunk(Chunk const&) = delete;
  Chunk& operator=(Chunk const&) = delete;

  Mesh const& getMesh();
  glm::ivec2 getPosition() const;
  Block* getBlock(glm::ivec3 pos);

private:
  void generateMesh();
  std::array<GLfloat, 4> genOcclusion(glm::ivec3 pos, BlockFace face);
  bool isSolid(glm::ivec3 pos);

  glm::ivec2 chunkPos;
  Blocks blocks;

  // the gl mesh and corresponding data.
  Mesh* mesh;
  std::vector<GLuint> indices;
  std::vector<GLfloat> positions, normals, textureCoords, occlusion;
};
