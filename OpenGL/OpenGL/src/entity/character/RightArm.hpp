#pragma once

#include "../Member.hpp"
#include "../Cube.hpp"

static const std::vector<GLfloat> rightArmUVs= {
  //TOP
  10 / 16.f, 13 / 16.f,
  9 / 16.f, 13 / 16.f,
  9 / 16.f, 12 / 16.f,
  10 / 16.f, 12 / 16.f,
  // BOTTOM
  11 / 16.f, 13 / 16.f,
  10 / 16.f, 13 / 16.f,
  10 / 16.f, 12 / 16.f,
  11 / 16.f, 12 / 16.f,
  // FRONT
  10 / 16.f, 13 / 16.f,
  9 / 16.f, 13 / 16.f,
  9 / 16.f, 16 / 16.f,
  10 / 16.f, 16 / 16.f,
  // RIGHT
  11 / 16.f, 13 / 16.f,
  10 / 16.f, 13 / 16.f,
  10 / 16.f, 16 / 16.f,
  11 / 16.f, 16 / 16.f,
  // BACK
  12 / 16.f, 13 / 16.f,
  11 / 16.f, 13 / 16.f,
  11 / 16.f, 16 / 16.f,
  12 / 16.f, 16 / 16.f,
  // LEFT
  9 / 16.f, 13 / 16.f,
  8 / 16.f, 13 / 16.f,
  8 / 16.f, 16 / 16.f,
  9 / 16.f, 16 / 16.f,
};

class RightArm : public Member {

public:
  RightArm() {
    glm::mat4 I(1.f);

    geometryModel = glm::scale(I, {4, 12, 4});
    geometryModel = glm::translate(I, {0, -4, 0}) * geometryModel;

    node.loc = {-6, 4, 0};
  }

protected:
  std::unique_ptr<Mesh> createMesh() override {
    return std::make_unique<Mesh>(Cube::vertices, Cube::normals, rightArmUVs, Cube::occlusions, Cube::indices, Cube::normalMap);
  }
};
