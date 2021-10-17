#pragma once

#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "gl/Mesh.hpp"
#include "DefaultGeometry.hpp"

enum class BlockFace;

class CactusGeometry : public DefaultBlockGeometry {
public:
	static CactusGeometry* get();
	void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const override;

private:
	CactusGeometry();
	void genFace(glm::ivec3 pos, BlockFace face, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const;

	static const BlockData<3> blockPositions;
};
