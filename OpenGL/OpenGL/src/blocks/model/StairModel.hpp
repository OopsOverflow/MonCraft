#pragma once

#include "blocks/model/OrientableModel.hpp"
#include "blocks/model/BlockModel.hpp"

class StairModel : public OrientableModel {
public:
	static StairModel* get();

private:
	StairModel();

	static const QuadMesh<3> mesh;
	static const QuadMesh<2> UVMesh;
};
