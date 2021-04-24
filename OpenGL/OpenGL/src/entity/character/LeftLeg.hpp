#pragma once

#include "../Member.hpp"
#include "../Cube.hpp"

static const std::vector<GLfloat> leftLegUVs = {
  //TOP
  2 / 16.f, 4 / 16.f,
  1 / 16.f, 4 / 16.f,
  1 / 16.f, 5 / 16.f,
  2 / 16.f, 5 / 16.f,
  // BOTTOM
  3 / 16.f, 4 / 16.f,
  2 / 16.f, 4 / 16.f,
  2 / 16.f, 5 / 16.f,
  3 / 16.f, 5 / 16.f,
  // FRONT
  2 / 16.f, 5 / 16.f,
  1 / 16.f, 5 / 16.f,
  1 / 16.f, 8 / 16.f,
  2 / 16.f, 8 / 16.f,
  // RIGHT
  1 / 16.f, 5 / 16.f,
  0 / 16.f, 5 / 16.f,
  0 / 16.f, 8 / 16.f,
  1 / 16.f, 8 / 16.f,
  // BACK
  4 / 16.f, 5 / 16.f,
  3 / 16.f, 5 / 16.f,
  3 / 16.f, 8 / 16.f,
  4 / 16.f, 8 / 16.f,
  // LEFT
  3 / 16.f, 5 / 16.f,
  2 / 16.f, 5 / 16.f,
  2 / 16.f, 8 / 16.f,
  3 / 16.f, 8 / 16.f,
};

class LeftLeg : public Member {

public:
  LeftLeg()
   : Member(Mesh(Cube::vertices, Cube::normals, leftLegUVs, Cube::occlusions, Cube::indices))
  { }
};
