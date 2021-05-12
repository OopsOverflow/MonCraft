#pragma once

#include "BlockGeometry.hpp"

class TallgrassGeometry : public BlockGeometry {

public:
  static TallgrassGeometry* get();
  void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const override;

private:
  static const std::array<face_t<3>, 4> positions;
  static const std::array<face_t<3>, 4> normals;
  static const std::array<std::array<GLfloat, 4>, 4> occlusions;
};
