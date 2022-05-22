#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity/Member.hpp"
#include "entity/Cube.hpp"
#include "gl/Mesh.hpp"

static const std::vector<GLfloat> rightLegUVs = {
  //TOP
  6 / 16.f, 13 / 16.f,
  5 / 16.f, 13 / 16.f,
  5 / 16.f, 12 / 16.f,
  6 / 16.f, 12 / 16.f,
  // BOTTOM
  7 / 16.f, 13 / 16.f,
  6 / 16.f, 13 / 16.f,
  6 / 16.f, 12 / 16.f,
  7 / 16.f, 12 / 16.f,
  // FRONT
  6 / 16.f, 13 / 16.f,
  5 / 16.f, 13 / 16.f,
  5 / 16.f, 16 / 16.f,
  6 / 16.f, 16 / 16.f,
  // RIGHT
  7 / 16.f, 13 / 16.f,
  6 / 16.f, 13 / 16.f,
  6 / 16.f, 16 / 16.f,
  7 / 16.f, 16 / 16.f,
  // BACK
  8 / 16.f, 13 / 16.f,
  7 / 16.f, 13 / 16.f,
  7 / 16.f, 16 / 16.f,
  8 / 16.f, 16 / 16.f,
  // LEFT
  5 / 16.f, 13 / 16.f,
  4 / 16.f, 13 / 16.f,
  4 / 16.f, 16 / 16.f,
  5 / 16.f, 16 / 16.f,
};

class RightLeg : public Member {

public:
  RightLeg() {
    const glm::mat4 I(1.f);

    geometryModel = glm::scale(I, {4, 12, 4});
    geometryModel = glm::translate(I, {0, -6, 0}) * geometryModel;

    const float zFightingOffset = 0.2f; // offset the legs slightly inwards and backwards
    geometryModel = glm::translate(I, {zFightingOffset, 0, -zFightingOffset}) * geometryModel;
    //rotate leg by 90 degrees
    glm::quat rot = glm::quat({-glm::pi<float>()/2.f, 0.f, 0.f});
    geometryModel = (glm::mat4)glm::mat4_cast(rot) * geometryModel;

    node.loc = {-2, -6, 0};
  }

protected:
  std::unique_ptr<Mesh> createMesh() override {
    return std::make_unique<Mesh>(Cube::vertices, Cube::normals, rightLegUVs, Cube::occlusions, Cube::indices, Cube::normalMap);
  }
};
