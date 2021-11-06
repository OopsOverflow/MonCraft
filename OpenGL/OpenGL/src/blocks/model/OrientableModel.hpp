#pragma once

#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "gl/Mesh.hpp"
#include "DefaultModel.hpp"

class Orientable_Block;

enum class BlockFace;

class OrientableModel : public DefaultBlockModel {
public:
	static OrientableModel* get();
	void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const override;

protected:
	OrientableModel(QuadMesh<3> mesh, QuadMesh<2> UVMesh);

private:
	void genFace(glm::ivec3 pos, BlockFace face, Orientable_Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const;

	std::array<size_t, 7> quadCount;
	const std::array<BlockMeshData, 4> blockPositions;
	const QuadMesh<2> blockUVs;
	const std::array<BlockMeshData, 4> blockNormals;
};
