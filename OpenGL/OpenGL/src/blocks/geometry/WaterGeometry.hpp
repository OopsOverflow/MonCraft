#pragma once

#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "gl/Mesh.hpp"
#include "BlockGeometry.hpp"

enum class BlockFace;

class WaterGeometry : public BlockGeometry {
public:
	static WaterGeometry* get();
	void generateMesh(glm::ivec3 pos, Block* block, MeshData& data) const override;

private:
	WaterGeometry();
	void genFace(glm::ivec3 pos, BlockFace face, Block* block, MeshData& data) const;

	static const BlockData<3> blockPositions;
	static const BlockData<3> filledBlockPositions;
	static const BlockData<3> fillSpaceBlockPositions;
	static const BlockData<3> blockNormals;
	static const BlockData<3> invertBlockNormals;
	static const face_t<2> faceNormalMap;
	static const std::array<std::pair<int, BlockFace>, 6> blockFaceOffsets;
	static const std::array<std::array<int, 3>, 4> checkNeighbors;
};
