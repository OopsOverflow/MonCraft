#pragma once

#include <glm/glm.hpp>
#include <array>
#include <utility>

#include "blocks/model/BlockModel.hpp"

class Block;
struct MeshData;

enum class BlockFace;

class DefaultBlockModel : public BlockModel {
public:
  static DefaultBlockModel* get();
  void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const override;

protected:
  DefaultBlockModel();
  FaceData<1> genOcclusion(glm::ivec3 pos, std::array<Block*, 26> const& neighbors, BlockFace face) const;
  void genFace(glm::ivec3 pos, BlockFace face, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const;

  static const QuadMesh<3> mesh;
  static const QuadMesh<3> meshNormals;
  static const Quad<2> faceUVs;

  static const BlockMeshData blockPositions;
  static const BlockMeshData blockNormals;

  static const FaceData<2> faceNormalMap;

  static const std::array<std::array<int, 9>, 6> blockOcclusionOffsets;
  static const std::array<std::pair<int, BlockFace>, 6> blockFaceOffsets;
};
