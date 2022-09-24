#pragma once

#include "gl/Camera.hpp"
#include "terrain/ChunkMesh.hpp"

class World;

// pair of z-distance, chunk
using ChunkList = std::vector<std::pair<float, std::shared_ptr<ChunkMesh>>>;

class Renderer {

public:
  Renderer();
  
  /**
  * List of visible chunks, sorted back-to-front, wrt. the z-distance.
  */
  ChunkList visibleChunks(Camera const& camera);
  
  /**
  * Renders the visible chunks.
  */
  void render(Camera const& camera, ChunkList const& chunks);

 /**
  * Renders the visible chunks as solid.
  */
  void renderSolid(ChunkList const& chunks);
  
  /**
   * Renders the chunk borders as wireframe.
   * (debug display)
   */
  void renderWireframe(Camera const& camera);

private:
  
  World& world;
};
