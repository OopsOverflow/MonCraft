#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "gl/Mesh.hpp"
#include "util/DataStore.hpp"
#include "blocks/Block.h"

/**
* Describes a subdivision of the terrain.
*/

class Chunk : public DataStore<Block::unique_ptr_t, 3> {
public:
  Chunk(int size);
};


/**
 * A Chunk holds a portion of the terrain data and manages a mesh accordingly.
 */

class ChunkMesh {
public:
  ChunkMesh(glm::ivec3 chunkPos, std::shared_ptr<Chunk> chunks);
  ~ChunkMesh();

  ChunkMesh(ChunkMesh const&) = delete;
  ChunkMesh& operator=(ChunkMesh const&) = delete;

  Mesh const& getMesh();
  Block* getBlock(glm::ivec3 pos);
  void setBlock(glm::ivec3 pos, Block::unique_ptr_t block);

private:
  void generateMeshData();
  void updateMesh();
  std::array<GLfloat, 4> genOcclusion(glm::ivec3 pos, BlockFace face);
  bool isSolid(glm::ivec3 pos);

  std::shared_ptr<Chunk> chunk;

  // the gl mesh and corresponding data.
  glm::ivec3 chunkPos;
  Mesh* mesh;
  std::vector<GLuint> indices;
  std::vector<GLfloat> positions, normals, textureCoords, occlusion;
};
