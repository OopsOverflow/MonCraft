#pragma once

#include "../Member.hpp"
#include "../Cube.hpp"

static const std::vector<GLfloat> chestUVs= {
   // TOP
   7 / 16.f, 5 / 16.f,
   5 / 16.f, 5 / 16.f,
   5 / 16.f, 4 / 16.f,
   7 / 16.f, 4 / 16.f,
   // BOTTOM
   9 / 16.f, 5 / 16.f,
   7 / 16.f, 5 / 16.f,
   7 / 16.f, 4 / 16.f,
   9 / 16.f, 4 / 16.f,
   // FRONT
   7 / 16.f, 5 / 16.f,
   5 / 16.f, 5 / 16.f,
   5 / 16.f, 8 / 16.f,
   7 / 16.f, 8 / 16.f,
   // RIGHT
   8 / 16.f, 5 / 16.f,
   6 / 16.f, 5 / 16.f,
   6 / 16.f, 8 / 16.f,
   8 / 16.f, 8 / 16.f,
   // BACK
   10 / 16.f, 5 / 16.f,
   8  / 16.f, 5 / 16.f,
   8  / 16.f, 8 / 16.f,
   10 / 16.f, 8 / 16.f,
   // LEFT
   5 / 16.f, 5 / 16.f,
   4 / 16.f, 5 / 16.f,
   4 / 16.f, 8 / 16.f,
   5 / 16.f, 8 / 16.f,
};

class Chest : public Member {

public:
  Chest() {
    glm::mat4 I(1.f);

    geometryModel = glm::scale(I, {8, 12, 4});
  }

protected:
  std::unique_ptr<Mesh> createMesh() override {
    return std::make_unique<Mesh>(Cube::vertices, Cube::normals, chestUVs, Cube::occlusions, Cube::indices, Cube::normalMap);
  }
};
