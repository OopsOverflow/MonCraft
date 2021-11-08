#pragma once

#include <GL/glew.h>
#include <array>
#include <glm/glm.hpp>

#include "blocks/model/DefaultModel.hpp"
#include "blocks/model/BlockModel.hpp"

class Block;
struct MeshData;

enum class BlockFace;

class WaterModel : public DefaultBlockModel {
public:
	static WaterModel* get();
	void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const override;

private:
	WaterModel();
	std::array<GLfloat, 4> genOcclusion(glm::ivec3 pos, std::array<Block*, 26> const& neighbors, BlockFace face) const;
	void genFace(glm::ivec3 pos, BlockFace face, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const;

	static const BlockData<3> blockPositions;
	static const BlockData<3> filledBlockPositions;
	static const BlockData<3> fillSpaceBlockPositions;
	static const BlockData<3> blockNormals;
	static const BlockData<3> invertBlockNormals;
	static const face_t<2> faceNormalMap;
	static const std::array<std::array<int, 8>, 6> blockOcclusionOffsets;
	static const std::array<std::pair<int, BlockFace>, 6> blockFaceOffsets;
	static const std::array<std::array<int, 3>, 4> checkNeighbors;
};
