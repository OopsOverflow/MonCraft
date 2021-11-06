#pragma once

#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "gl/Mesh.hpp"
#include "DefaultModel.hpp"

enum class BlockFace;

class CactusModel : public DefaultBlockModel {
public:
	static CactusModel* get();
	void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const override;

private:
	CactusModel();
	void genFace(glm::ivec3 pos, BlockFace face, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const;

	static const BlockData<3> blockPositions;
};
