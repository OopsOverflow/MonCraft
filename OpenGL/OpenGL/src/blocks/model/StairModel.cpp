#include "StairModel.hpp"

using namespace glm;

StairModel::StairModel()
  : OrientableModel(mesh, UVMesh)
{}

StairModel* StairModel::get() {
  static StairModel inst;
  return &inst;
}

/// below is all the data and lookup tables

const QuadMesh<3> StairModel::mesh = {
  std::vector<Quad<3>> { // TOP
    { vec3
      { -0.5f,  0.5f,  0.0f },
      {  0.5f,  0.5f,  0.0f },
      {  0.5f,  0.5f, -0.5f },
      { -0.5f,  0.5f, -0.5f },
    }
  }, { // BOTTOM
    { vec3
      {  0.5f, -0.5f,  0.5f },
      { -0.5f, -0.5f,  0.5f },
      { -0.5f, -0.5f, -0.5f },
      {  0.5f, -0.5f, -0.5f },
    }
  }, { // FRONT
    { vec3
      {  0.5f,  0.0f,  0.5f },
      { -0.5f,  0.0f,  0.5f },
      { -0.5f, -0.5f,  0.5f },
      {  0.5f, -0.5f,  0.5f },
    }
  }, { // RIGHT
    { vec3 // top
      { 0.5f,  0.5f, -0.5f },
      { 0.5f,  0.5f,  0.0f },
      { 0.5f,  0.0f,  0.0f },
      { 0.5f,  0.0f, -0.5f },
    }, { vec3 // bottom
      { 0.5f,  0.0f, -0.5f },
      { 0.5f,  0.0f,  0.5f },
      { 0.5f, -0.5f,  0.5f },
      { 0.5f, -0.5f, -0.5f },
    }
  }, { // BACK
    { vec3
      { -0.5f,  0.5f, -0.5f },
      {  0.5f,  0.5f, -0.5f },
      {  0.5f, -0.5f, -0.5f },
      { -0.5f, -0.5f, -0.5f },
    }
  }, { // LEFT
    { vec3 // top
      { -0.5f,  0.5f,  0.0f },
      { -0.5f,  0.5f, -0.5f },
      { -0.5f,  0.0f, -0.5f },
      { -0.5f,  0.0f,  0.0f },
    }, { vec3 // bottom
      { -0.5f,  0.0f,  0.5f },
      { -0.5f,  0.0f, -0.5f },
      { -0.5f, -0.5f, -0.5f },
      { -0.5f, -0.5f,  0.5f },
    }
  }, { // INNER
    { vec3 // top
      { -0.5f,  0.0f,  0.5f },
      {  0.5f,  0.0f,  0.5f },
      {  0.5f,  0.0f,  0.0f },
      { -0.5f,  0.0f,  0.0f },
    }, { vec3 // front
      {  0.5f,  0.5f,  0.0f },
      { -0.5f,  0.5f,  0.0f },
      { -0.5f,  0.0f,  0.0f },
      {  0.5f,  0.0f,  0.0f },
    }
  }
};

const QuadMesh<2> StairModel::UVMesh = {
  std::vector<Quad<2>> { // TOP
    { vec2
      { 1.0f, 0.5f },
      { 0.0f, 0.5f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }, { // BOTTOM
    { vec2
      { 1.0f, 0.5f },
      { 0.0f, 0.5f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }, { // FRONT
    { vec2
      { 1.0f, 0.0f },
      { 0.0f, 0.0f },
      { 0.0f, 0.5f },
      { 1.0f, 0.5f },
    }
  }, { // RIGHT
    { vec2 // top
      { 1.0f, 0.0f },
      { 0.5f, 0.0f },
      { 0.5f, 0.5f },
      { 1.0f, 0.5f },
    }, { vec2 // bottom
      { 1.0f, 0.5f },
      { 0.0f, 0.5f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }, { // BACK
    { vec2
      { 1.0f, 0.0f },
      { 0.0f, 0.0f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }, { // LEFT
    { vec2 // top
      { 0.5f, 0.0f },
      { 0.0f, 0.0f },
      { 0.0f, 0.5f },
      { 0.5f, 0.5f },
    }, { vec2 // bottom
      { 1.0f, 0.5f },
      { 0.0f, 0.5f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }, { // INNER
    { vec2 // top
      { 1.0f, 0.0f },
      { 0.0f, 0.0f },
      { 0.0f, 0.5f },
      { 1.0f, 0.5f },
    }, { vec2 // front
      { 1.0f, 0.5f },
      { 0.0f, 0.5f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
    }
  }
};
