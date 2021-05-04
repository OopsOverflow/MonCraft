#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <mutex>

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

  Block* getBlock(glm::ivec3 pos);
  void setBlock(glm::ivec3 pos, Block::unique_ptr_t block);
  bool isLoaded();

  void compute();
  void draw();

  std::array<std::weak_ptr<Chunk>, 26> neighbors;

private:
  void update();
  bool isSolid(glm::ivec3 pos);
  bool isSolidNoChecks(glm::ivec3 pos);

  glm::ivec3 chunkPos;
  // the gl mesh and corresponding data.
  std::unique_ptr<Mesh> mesh;
  std::vector<GLuint> indices;
  std::vector<GLfloat> positions, normals, textureCoords, occlusion;

  std::mutex computeMutex;
  bool computed;
};
