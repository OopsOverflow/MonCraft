#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity/Member.hpp"
#include "entity/Cube.hpp"
#include "gl/Mesh.hpp"

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

static const std::vector<std::pair<float, glm::vec3> > rightArmKeyframes = {
  {0.f, {0.f, -1.f, 0.f}},
  {2.f/ 3.f, {-0.05f, -1.f, 0.005f}},
  {4.f/ 3.f, {0.05f, -1.f, 0.005f}},
  {2.f, {0.f, -1.f, 0.f}},
  {8.f/ 3.f, {-0.05f, -1.f, -0.005f}},
  {10.f/ 3.f, {0.05f, -1.f, -0.005f}},
  {4.f, {0.f, -1.f, 0.f}},

};

class RightArm : public Member {

public:
  RightArm() {
    const glm::mat4 I(1.f);

    geometryModel = glm::scale(I, {4, 12, 4});
    geometryModel = glm::translate(I, {0, -4, 0}) * geometryModel;
    //rotate leg by 90 degrees
    glm::quat rot = glm::quat({-glm::pi<float>()/2.f, 0.f, 0.f});
    geometryModel = (glm::mat4)glm::mat4_cast(rot) * geometryModel;

    node.loc = {-6, 4, 0};

    glm::vec3 line = (rightArmKeyframes[5].second - rightArmKeyframes[1].second) * smoothing;
    Spline idleAnim(rightArmKeyframes, -line, line);
    anim = std::make_unique<Animation>(idleAnim);
  }

protected:
  std::unique_ptr<Mesh> createMesh() override {
    return std::make_unique<Mesh>(Cube::vertices, Cube::normals, rightArmUVs, Cube::occlusions, Cube::indices, Cube::normalMap);
  }
};
