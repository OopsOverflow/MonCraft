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

using BlockMeshData = std::array<std::vector<GLfloat>, 7>;

template<glm::length_t L>
using Quad = std::array<glm::vec<L, float, glm::defaultp>, 4>;

template<glm::length_t L>
using QuadMesh = std::array<std::vector<Quad<L>>, 7>;

/**
 * A block model is responsible to generate a block mesh components based on
 * the neighboring blocks. See DefaultModel for the standard cube model.
 * This flexibility allows us to generate any model for a block, such as
 * the tallgrass (cross-pattern) or the water (smaller than a block)
 */

class BlockModel {
public:
  virtual ~BlockModel();

  /**
   * Generates the mesh part caused by a given block.
   * Writes the data in the MeshData passed by reference.
   */
  virtual void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const = 0;

protected:
  BlockModel();

  template<glm::length_t L>
  static Quad<L> transform(Quad<L> quad, glm::mat<L+1, L+1, glm::f32, glm::defaultp> const& transform);

  template<glm::length_t L>
  static std::vector<Quad<L>> transform(std::vector<Quad<L>> quads, glm::mat<L+1, L+1, glm::f32, glm::defaultp> const& transform);

  template<glm::length_t L>
  static QuadMesh<L> transform(QuadMesh<L> mesh, glm::mat<L+1, L+1, glm::f32, glm::defaultp> const& transform);

  template<glm::length_t L>
  static std::vector<GLfloat> flatten(std::vector<Quad<L>> const& quads);

  template<glm::length_t L>
  static BlockMeshData flatten(QuadMesh<L> const& mesh);

  static std::vector<GLfloat> computeUV(glm::vec2 index, std::vector<Quad<2>> quads);

  template<glm::length_t L>
  static std::array<std::vector<GLfloat>, 4> computeFacing(std::vector<Quad<L>> const& quads);

  template<>
  std::array<std::vector<GLfloat>, 4> computeFacing(std::vector<Quad<1>> const& quads);

  template<glm::length_t L>
  static std::array<BlockMeshData, 4> computeFacing(QuadMesh<L> const& mesh);

  template<>
  std::array<BlockMeshData, 4> computeFacing(QuadMesh<1> const& mesh);

  static const std::array<glm::mat4, 4> facingTransforms;
};
