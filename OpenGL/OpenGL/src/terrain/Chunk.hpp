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

class BlockDeleter {
public:
  void operator()(Block* block) const {
    if(!block->isStatic())
      delete block;
  }
};

class Blocks : public DataStore<std::unique_ptr<Block, BlockDeleter>, 3> {
public:
  Blocks(int size)
      : DataStore<std::unique_ptr<Block, BlockDeleter>, 3>(glm::ivec3(size))
  {
    // static int count = 0;
    // static int totalSize = 0;
    // totalSize += size * size * size * (sizeof(Block) + sizeof(std::unique_ptr<Block>));
    // std::cout << "Blocks: " << ++count << "(" << totalSize << ")" << std::endl;
  }

  template<class T, class... Args>
  static
  typename std::enable_if<std::is_base_of<Block, T>::value, std::unique_ptr<Block, BlockDeleter>>::type
  create_static() {
    return std::unique_ptr<Block, BlockDeleter>(T::get());
  }

  template<class T, class... Args>
  static
  typename std::enable_if<std::is_base_of<Block, T>::value, std::unique_ptr<Block, BlockDeleter>>::type
  create_dynamic(Args&&... args) {
    return std::unique_ptr<Block, BlockDeleter>(new T(std::forward<Args>(args)...));
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
  void unload();

private:
  void generateMesh();
  std::array<GLfloat, 4> genOcclusion(glm::ivec3 pos, BlockFace face);
  bool isSolid(glm::ivec3 pos);

  glm::ivec3 chunkPos;
  Blocks* blocks;

  // the gl mesh and corresponding data.
  Mesh* mesh;
  std::vector<GLuint> indices;
  std::vector<GLfloat> positions, normals, textureCoords, occlusion;
};
