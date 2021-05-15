#pragma once

#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "gl/Mesh.hpp"
#include "BlockGeometry.hpp"

enum class BlockFace;

class DefaultBlockGeometry : public BlockGeometry {
public:
  static DefaultBlockGeometry* get();
  void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const override;

private:
  DefaultBlockGeometry();
  std::array<GLfloat, 4> genOcclusion(glm::ivec3 pos, std::array<Block*, 26> const& neighbors, BlockFace face) const;
  void genFace(glm::ivec3 pos, BlockFace face, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const;

  static const BlockData<3> blockPositions;
  static const BlockData<3> blockNormals;
  static const face_t<2> faceNormalMap;
  static const std::array<std::array<int, 8>, 6> blockOcclusionOffsets;
  static const std::array<std::pair<int, BlockFace>, 6> blockFaceOffsets;
};
