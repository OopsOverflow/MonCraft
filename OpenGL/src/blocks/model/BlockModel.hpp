#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stddef.h>
#include <array>
#include <vector>

class Block;
struct MeshData;
enum class BlockFace;

// corners are ordered in CCW rotation when looking at the face, with the first corner
// corner being located:
//  - -x, +z for the top face
//  - +x, +z for the bottom face
//  - +x, +y for the front face
//  - +y, -z for the right face
//  - -x, -y for the back face
//  - +y, +z for the left face
template<size_t N>
using FaceData = std::array<GLfloat, 4 * N>;

// faces are in following order: top, bottom, front, right, back, left
template<size_t N>
using BlockData = std::array<FaceData<N>, 6>;

// compared to BlockData, an additional "internal face" for all geometry not on the side
// of the block.
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

  static Quad<3> normals(Quad<3> quad);
  static std::vector<Quad<3>> normals(std::vector<Quad<3>> quads);
  static QuadMesh<3> normals(QuadMesh<3> mesh);

  template<glm::length_t L>
  static std::vector<GLfloat> flatten(Quad<L> const& quad);

  template<glm::length_t L>
  static std::vector<GLfloat> flatten(std::vector<Quad<L>> const& quads);

  template<glm::length_t L>
  static BlockMeshData flatten(QuadMesh<L> const& mesh);
  
  static glm::vec2 toFaceCoordinates(BlockFace face, glm::vec3 pos);
  
  // bilinear interpolation of quad values, where pos is in face coords (range -0.5, 0.5)
  static GLfloat lerp(FaceData<1> quad, glm::vec2 pos);

  static std::vector<GLfloat> computeUV(glm::vec2 index, Quad<2> quad);

  static std::vector<GLfloat> computeUV(glm::vec2 index, std::vector<Quad<2>> quads);

  template<glm::length_t L>
  static std::array<std::vector<GLfloat>, 4> computeFacing(std::vector<Quad<L>> const& quads);

  template<glm::length_t L>
  static std::array<BlockMeshData, 4> computeFacing(QuadMesh<L> const& mesh);

  // transforms block 3D coordinates to another face.
  static const std::array<glm::mat4, 4> facingTransforms;
  
  // transforms block 3D coordinates (range -0.5, 0.5) to coordinates on the front face.
  // that way, extracting x and y gives the face local coordinate.
  // (top-right corner is (0.5, 0.5)
  static const std::array<glm::mat4, 6> faceTransforms;
};


template<>
std::array<std::vector<GLfloat>, 4> BlockModel::computeFacing(std::vector<Quad<1>> const& quads);


template<>
std::array<BlockMeshData, 4> BlockModel::computeFacing(QuadMesh<1> const& mesh);
