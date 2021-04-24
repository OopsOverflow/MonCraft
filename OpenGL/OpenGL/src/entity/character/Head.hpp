#pragma once

#include "../Member.hpp"
#include "../Cube.hpp"

static const std::vector<GLfloat> headUVs = {
  //TOP
  4 / 16.f, 0 / 6.f,
  2 / 16.f, 0 / 16.f,
  2 / 16.f, 2 / 16.f,
  4 / 16.f, 2 / 16.f,
  // BOTTOM
  6 / 16.f, 0 / 16.f,
  4 / 16.f, 0 / 16.f,
  4 / 16.f, 2 / 16.f,
  6 / 16.f, 2 / 16.f,
  // FRONT
  4 / 16.f, 2 / 16.f,
  2 / 16.f, 2 / 16.f,
  2 / 16.f, 4 / 16.f,
  4 / 16.f, 4 / 16.f,
  // RIGHT
  2 / 16.f, 2 / 16.f,
  0 / 16.f, 2 / 16.f,
  0 / 16.f, 4 / 16.f,
  2 / 16.f, 4 / 16.f,
  // BACK
  8 / 16.f, 2 / 16.f,
  6 / 16.f, 2 / 16.f,
  6 / 16.f, 4 / 16.f,
  8 / 16.f, 4 / 16.f,
  // LEFT
  6 / 16.f, 2 / 16.f,
  4 / 16.f, 2 / 16.f,
  4 / 16.f, 4 / 16.f,
  6 / 16.f, 4 / 16.f,
};

class Head : public Member {

public:
  Head()
   : Member(Mesh(Cube::vertices, Cube::normals, headUVs, Cube::occlusions, Cube::indices))
  { }
};
