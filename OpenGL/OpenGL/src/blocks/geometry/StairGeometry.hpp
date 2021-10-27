#pragma once

#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "gl/Mesh.hpp"
#include "DefaultGeometry.hpp"

class Stair_Block;

enum class BlockFace;

class StairGeometry : public DefaultBlockGeometry {
public:
	static StairGeometry* get();
	void generateMesh(glm::ivec3 pos, Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const override;

private:
	StairGeometry();
	void genFace(glm::ivec3 pos, BlockFace face, Stair_Block* block, std::array<Block*, 26> const& neighbors, MeshData& data) const;

	static const QuadMesh<3> mesh;
	static const QuadMesh<2> UVMesh;

	static const std::array<BlockMeshData, 4> blockPositions;
	static const std::array<std::vector<GLfloat>, 4> blockInnerOcclusions;
	static const std::array<std::vector<GLfloat>, 4> blockInnerNormals;

	static const std::vector<Quad<1>> innerOcclusions;
	static const std::vector<Quad<3>> innerNormals;
};
