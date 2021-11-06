#include <algorithm>

#include "BlockModel.hpp"
#include "blocks/Block.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::mat4;
using glm::vec3;
using glm::pi;
using glm::half_pi;

BlockModel::BlockModel() {}
BlockModel::~BlockModel() {}

template<glm::length_t L>
Quad<L> BlockModel::transform(Quad<L> quad, glm::mat<L+1, L+1, glm::f32, glm::defaultp> const& tr) {
  std::transform(quad.begin(), quad.end(), quad.begin(), [&](auto vert) {
    return tr * glm::vec<L+1, float, glm::defaultp>(vert, 1.f);
  });

  return quad;
}

template<glm::length_t L>
std::vector<Quad<L>> BlockModel::transform(std::vector<Quad<L>> quads, glm::mat<L+1, L+1, glm::f32, glm::defaultp> const& tr) {
  std::transform(quads.begin(), quads.end(), quads.begin(), [&](auto quad) {
    return transform(quad, tr);
  });

  return quads;
}

template<glm::length_t L>
QuadMesh<L> BlockModel::transform(QuadMesh<L> mesh, glm::mat<L+1, L+1, glm::f32, glm::defaultp> const& tr) {
  for(auto& quads : mesh) {
    quads = transform(quads, tr);
  }

  return mesh;
}

template<glm::length_t L>
std::vector<GLfloat> BlockModel::flatten(std::vector<Quad<L>> const& quads) {
  size_t size = quads.size() * 4 * L;
  std::vector<GLfloat> res;
  res.reserve(size);

  for(Quad<L> const& quad : quads) {
    GLfloat *data = (GLfloat*)quad.data();
    std::copy(data, data + (4 * L), std::back_inserter(res));
  }

  return res;
}

template<glm::length_t L>
BlockMeshData BlockModel::flatten(QuadMesh<L> const& mesh) {
  BlockMeshData res;

  for(size_t i = 0; i < mesh.size(); ++i) {
    res[i] = flatten(mesh[i]);
  }

  return res;
}

std::vector<GLfloat> BlockModel::computeUV(glm::vec2 index, std::vector<Quad<2>> quads) {
  static const int atlasSize = 8.f;
  glm::mat3 tr = glm::mat3(1.f);
  tr = glm::scale(tr, glm::vec2(1.f / atlasSize));
  tr = glm::translate(tr, index);
  return flatten(transform(quads, tr));
}

const std::array<mat4, 4> BlockModel::facingTransforms = {
  mat4(1.f), // NORTH
  rotate(mat4(1.f), pi<float>(), vec3(0, 1, 0)), // SOUTH
  rotate(mat4(1.f), -half_pi<float>(), vec3(0, 1, 0)), // EAST
  rotate(mat4(1.f), half_pi<float>(), vec3(0, 1, 0)), // WEST
};

template<glm::length_t L>
std::array<std::vector<GLfloat>, 4> BlockModel::computeFacing(std::vector<Quad<L>> const& quads) {
  std::array<std::vector<GLfloat>, 4> res;

  for(size_t i = 0; i < res.size(); i++) {
    res[i] = flatten(transform(quads, facingTransforms[i]));
  }

  return res;
}

template<>
std::array<std::vector<GLfloat>, 4> BlockModel::computeFacing(std::vector<Quad<1>> const& quads) {
  std::array<std::vector<GLfloat>, 4> res;

  for(size_t i = 0; i < res.size(); i++) {
    res[i] = flatten(quads);
  }

  return res;
}

template<glm::length_t L>
std::array<BlockMeshData, 4> BlockModel::computeFacing(QuadMesh<L> const& mesh) {
  std::array<BlockMeshData, 4> res;

  for(size_t i = 0; i < res.size(); i++) {
    res[i] = flatten(transform(mesh, facingTransforms[i]));
  }

  return res;
}

template<>
std::array<BlockMeshData, 4> BlockModel::computeFacing(QuadMesh<1> const& mesh) {
  std::array<BlockMeshData, 4> res;

  for(size_t i = 0; i < res.size(); i++) {
    res[i] = flatten(mesh);
  }

  return res;
}

template
Quad<2> BlockModel::transform(Quad<2> quads, glm::mat3 const& tr);
template
Quad<3> BlockModel::transform(Quad<3> quads, glm::mat4 const& tr);

template
std::vector<Quad<2>> BlockModel::transform(std::vector<Quad<2>> quads, glm::mat3 const& tr);
template
std::vector<Quad<3>> BlockModel::transform(std::vector<Quad<3>> quads, glm::mat4 const& tr);

template
QuadMesh<2> BlockModel::transform(QuadMesh<2> mesh, glm::mat3 const& tr);
template
QuadMesh<3> BlockModel::transform(QuadMesh<3> mesh, glm::mat4 const& tr);

template
std::vector<GLfloat> BlockModel::flatten(std::vector<Quad<1>> const& quads);
template
std::vector<GLfloat> BlockModel::flatten(std::vector<Quad<2>> const& quads);
template
std::vector<GLfloat> BlockModel::flatten(std::vector<Quad<3>> const& quads);

template
BlockMeshData BlockModel::flatten(QuadMesh<1> const& quads);
template
BlockMeshData BlockModel::flatten(QuadMesh<2> const& quads);
template
BlockMeshData BlockModel::flatten(QuadMesh<3> const& quads);

template
std::array<std::vector<GLfloat>, 4> BlockModel::computeFacing(std::vector<Quad<2>> const& quads);
template
std::array<std::vector<GLfloat>, 4> BlockModel::computeFacing(std::vector<Quad<3>> const& quads);

template
std::array<BlockMeshData, 4> BlockModel::computeFacing(QuadMesh<2> const& quads);
template
std::array<BlockMeshData, 4> BlockModel::computeFacing(QuadMesh<3> const& quads);
