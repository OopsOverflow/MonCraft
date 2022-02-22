#pragma once

#include <GL/glew.h>
#include <memory>
#include <mutex>
#include <glm/glm.hpp>

#include "terrain/Chunk.hpp"
#include "blocks/Block.hpp"
#include "gl/Mesh.hpp"

/**
* Describes a subdivision of the terrain.
*/

class ChunkMesh : public Chunk {
public:
  ChunkMesh(glm::ivec3 chunkPos, int chunkSize);
  ~ChunkMesh();

  ChunkMesh(ChunkMesh const&) = delete;
  ChunkMesh& operator=(ChunkMesh const&) = delete;

  /**
  * Sets the block at the given position.
  * The coordinate origin is in chunk space i.e. (0,0,0) is the chunk corner.
  * This will update the mesh and, if possible, neighboring meshes as well.
  */
  void setBlock(glm::ivec3 pos, Block::unique_ptr_t block);

  /**
  * Signals the chunk that it must be recomputed on next update.
  * The chunk will be recomputed only if it was loaded (see isComputed).
  */
  void markToRecompute();

  /**
  * If the chunk has been computed at least once.
  */
  bool isComputed();

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

  void drawAllAsSolid();

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

private:
  // the gl mesh and corresponding data.
  std::unique_ptr<Mesh> mesh;
  GLuint solidOffset = 0;
  glm::uvec3 transpOffset = glm::uvec3(0);
  MeshData meshData;

  bool loaded;
  bool mustRecompute;
  std::mutex computeMutex;
  glm::mat4 model;
};
