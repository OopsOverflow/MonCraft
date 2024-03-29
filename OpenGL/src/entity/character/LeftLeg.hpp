#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity/Member.hpp"
#include "entity/Cube.hpp"
#include "gl/Mesh.hpp"

static const std::vector<GLfloat> leftLegUVs = {
  //TOP
  2 / 16.f, 5 / 16.f,
  1 / 16.f, 5 / 16.f,
  1 / 16.f, 4 / 16.f,
  2 / 16.f, 4 / 16.f,
  // BOTTOM
  3 / 16.f, 5 / 16.f,
  2 / 16.f, 5 / 16.f,
  2 / 16.f, 4 / 16.f,
  3 / 16.f, 4 / 16.f,
  // FRONT
  2 / 16.f, 5 / 16.f,
  1 / 16.f, 5 / 16.f,
  1 / 16.f, 8 / 16.f,
  2 / 16.f, 8 / 16.f,
  // RIGHT
  3 / 16.f, 5 / 16.f,
  2 / 16.f, 5 / 16.f,
  2 / 16.f, 8 / 16.f,
  3 / 16.f, 8 / 16.f,
  // BACK
  4 / 16.f, 5 / 16.f,
  3 / 16.f, 5 / 16.f,
  3 / 16.f, 8 / 16.f,
  4 / 16.f, 8 / 16.f,
  // LEFT
  1 / 16.f, 5 / 16.f,
  0 / 16.f, 5 / 16.f,
  0 / 16.f, 8 / 16.f,
  1 / 16.f, 8 / 16.f,
};

static const Spline leftLegIdleAnim ({
  {0,    {0.f, -1.f, 0.f}},
  {750,  {0.f, -1.f, 0.005f}},
  {1500, {0.005f, -1.f, 0.005f}},
  {2250, {0.f, -1.f, -0.005f}},
  {3000, {0.f, -1.f, 0.f}},

});

static const Spline leftLegWalkAnim ({
  {0,    {0.f, -1.f, 1.0f}},
  {250,  {0.01f, -1.f, 0.f}},
  {500,  {0.f, -1.f, -1.0f}},
  {750,  {-0.01f, -1.f, 0.f}},
  {1000, {0.f, -1.f, 1.0f}},
  
});

class LeftLeg : public Member {

public:
  LeftLeg() {
    const glm::mat4 I(1.f);

    geometryModel = glm::scale(I, {4, 12, 4});
    geometryModel = glm::translate(I, {0, -6, 0}) * geometryModel;

    const float zFightingOffset = 0.2f; // offset the legs slightly inwards and backwards
    geometryModel = glm::translate(I, {zFightingOffset, 0, -zFightingOffset}) * geometryModel;
    //rotate arm by 90 degrees
    glm::quat rot = glm::quat({-glm::pi<float>()/2.f, 0.f, 0.f});
    geometryModel = (glm::mat4)glm::mat4_cast(rot) * geometryModel;

    node.loc = {2, -6, 0};

    anim = std::make_unique<AnimationMixer>(leftLegIdleAnim);
    anim->addAnim(Animation::Walk, leftLegWalkAnim);
  }

protected:
  std::unique_ptr<Mesh> createMesh() override {
    return std::make_unique<Mesh>(Cube::vertices, Cube::normals, leftLegUVs, Cube::occlusions, Cube::indices, Cube::normalMap);
  }
};