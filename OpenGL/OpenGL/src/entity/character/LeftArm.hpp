#pragma once

#include "../Member.hpp"
#include "../Cube.hpp"

static const std::vector<GLfloat> leftArmUVs= {
  //TOP
  12 / 16.f, 4 / 16.f,
  11 / 16.f, 4 / 16.f,
  11 / 16.f, 5 / 16.f,
  12 / 16.f, 5 / 16.f,
  // BOTTOM
  13 / 16.f, 4 / 16.f,
  12 / 16.f, 4 / 16.f,
  12 / 16.f, 5 / 16.f,
  13 / 16.f, 5 / 16.f,
  // FRONT
  12 / 16.f, 5 / 16.f,
  11 / 16.f, 5 / 16.f,
  11 / 16.f, 8 / 16.f,
  12 / 16.f, 8 / 16.f,
  // RIGHT
  11 / 16.f, 5 / 16.f,
  10 / 16.f, 5 / 16.f,
  10 / 16.f, 8 / 16.f,
  11 / 16.f, 8 / 16.f,
  // BACK
  14 / 16.f, 5 / 16.f,
  13 / 16.f, 5 / 16.f,
  13 / 16.f, 8 / 16.f,
  14 / 16.f, 8 / 16.f,
  // LEFT
  13 / 16.f, 5 / 16.f,
  12 / 16.f, 5 / 16.f,
  12 / 16.f, 8 / 16.f,
  13 / 16.f, 8 / 16.f,
};

class LeftArm : public Member {

public:
  LeftArm()
   : Member(Mesh(Cube::vertices, Cube::normals, leftArmUVs, Cube::occlusions, Cube::indices))
  { }
};
