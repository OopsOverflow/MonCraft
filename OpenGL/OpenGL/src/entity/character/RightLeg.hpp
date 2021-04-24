#pragma once

#include "../Member.hpp"
#include "../Cube.hpp"

static const std::vector<GLfloat> rightLegUVs = {
  //TOP
  6 / 16.f, 12 / 16.f,
  5 / 16.f, 12 / 16.f,
  5 / 16.f, 13 / 16.f,
  6 / 16.f, 13 / 16.f,
  // BOTTOM
  7 / 16.f, 12 / 16.f,
  6 / 16.f, 12 / 16.f,
  6 / 16.f, 13 / 16.f,
  7 / 16.f, 13 / 16.f,
  // FRONT
  6 / 16.f, 13 / 16.f,
  5 / 16.f, 13 / 16.f,
  5 / 16.f, 16 / 16.f,
  6 / 16.f, 16 / 16.f,
  // RIGHT
  5 / 16.f, 13 / 16.f,
  4 / 16.f, 13 / 16.f,
  4 / 16.f, 16 / 16.f,
  5 / 16.f, 16 / 16.f,
  // BACK
  8 / 16.f, 13 / 16.f,
  7 / 16.f, 13 / 16.f,
  7 / 16.f, 16 / 16.f,
  8 / 16.f, 16 / 16.f,
  // LEFT
  7 / 16.f, 13 / 16.f,
  6 / 16.f, 13 / 16.f,
  6 / 16.f, 16 / 16.f,
  7 / 16.f, 16 / 16.f,
};

class RightLeg : public Member {

public:
  RightLeg()
   : Member(Mesh(Cube::vertices, Cube::normals, rightLegUVs, Cube::occlusions, Cube::indices))
  { }
};
