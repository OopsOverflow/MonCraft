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

class BlockGeometry {
public:
  virtual ~BlockGeometry();
  virtual void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const = 0;

protected:
  BlockGeometry();
  face_t<2> genFaceUV(glm::ivec2 index) const;
};
