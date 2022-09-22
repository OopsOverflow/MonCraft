#pragma once

#include <GL/glew.h>
#include <array>
#include <glm/glm.hpp>

#include "blocks/model/DefaultModel.hpp"
#include "blocks/model/BlockModel.hpp"

class Block;
struct MeshData;

class TallgrassModel : public DefaultBlockModel {

public:
  static TallgrassModel* get();
  void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const override;

private:
  static const std::array<FaceData<3>, 4> positions;
  static const std::array<FaceData<3>, 4> normals;
  static const std::array<std::array<GLfloat, 4>, 4> occlusions;
  static const FaceData<2> faceNormalMap;
};
