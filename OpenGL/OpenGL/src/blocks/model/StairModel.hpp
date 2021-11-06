#pragma once

#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "gl/Mesh.hpp"
#include "OrientableModel.hpp"

class StairModel : public OrientableModel {
public:
	static StairModel* get();

private:
	StairModel();

	static const QuadMesh<3> mesh;
	static const QuadMesh<2> UVMesh;
};
