#pragma once

#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "gl/Mesh.hpp"

class Block;

template<size_t N>
using face_t = std::array<GLfloat, 4 * N>;

template<size_t N>
using BlockData = std::array<face_t<N>, 6>;

/**
 * A block geometry is responsible to generate a block mesh components based on
 * the neighboring blocks. See DefaultGeometry for the standard cube geometry.
 * This flexibility allows us to generate any geometry for a block, such as
 * the tallgrass (cross-pattern) or the water (smaller than a block)
 */

class BlockGeometry {
public:
  virtual ~BlockGeometry();

  /**
   * Generates the mesh part caused by a given block.
   * Writes the data in the MeshData passed by reference.
   */
  virtual void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const = 0;

protected:
  BlockGeometry();
  face_t<2> genFaceUV(glm::ivec2 index) const;
};
