#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity/Member.hpp"
#include "entity/Cube.hpp"
#include "gl/Mesh.hpp"

static const std::vector<GLfloat> headUVs= {
    // TOP
    2 / 16.f, 2 / 16.f,
    4 / 16.f, 2 / 16.f,
    4 / 16.f, 0 / 16.f,
    2 / 16.f, 0 / 16.f,
    // BOTTOM
    6 / 16.f, 2 / 16.f,
    4 / 16.f, 2 / 16.f,
    4 / 16.f, 0 / 16.f,
    6 / 16.f, 0 / 16.f,
    // FRONT
    4 / 16.f, 2 / 16.f,
    2 / 16.f, 2 / 16.f,
    2 / 16.f, 4 / 16.f,
    4 / 16.f, 4 / 16.f,
    // RIGHT
    6 / 16.f, 2 / 16.f,
    4 / 16.f, 2 / 16.f,
    4 / 16.f, 4 / 16.f,
    6 / 16.f, 4 / 16.f,
    // BACK
    8 / 16.f, 2 / 16.f,
    6 / 16.f, 2 / 16.f,
    6 / 16.f, 4 / 16.f,
    8 / 16.f, 4 / 16.f,
    // LEFT
    2 / 16.f, 2 / 16.f,
    0 / 16.f, 2 / 16.f,
    0 / 16.f, 4 / 16.f,
    2 / 16.f, 4 / 16.f,
};

class Head : public Member {

public:
  Head() {
    glm::mat4 I(1.f);

    geometryModel = glm::scale(I, {8, 8, 8});
    geometryModel = glm::translate(I, {0, 4, 0}) * geometryModel;

    node.loc = {0, 6, 0};
  }

protected:
  std::unique_ptr<Mesh> createMesh() override {
    return std::make_unique<Mesh>(Cube::vertices, Cube::normals, headUVs, Cube::occlusions, Cube::indices, Cube::normalMap);
  }
};
