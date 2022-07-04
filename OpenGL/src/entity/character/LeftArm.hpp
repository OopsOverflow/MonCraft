#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity/Member.hpp"
#include "entity/Cube.hpp"
#include "gl/Mesh.hpp"

static const std::vector<GLfloat> leftArmUVs = {
  //TOP
  12 / 16.f, 5 / 16.f,
  11 / 16.f, 5 / 16.f,
  11 / 16.f, 4 / 16.f,
  12 / 16.f, 4 / 16.f,
  // BOTTOM
  13 / 16.f, 5 / 16.f,
  12 / 16.f, 5 / 16.f,
  12 / 16.f, 4 / 16.f,
  13 / 16.f, 4 / 16.f,
  // FRONT
  12 / 16.f, 5 / 16.f,
  11 / 16.f, 5 / 16.f,
  11 / 16.f, 8 / 16.f,
  12 / 16.f, 8 / 16.f,
  // RIGHT
  13 / 16.f, 5 / 16.f,
  12 / 16.f, 5 / 16.f,
  12 / 16.f, 8 / 16.f,
  13 / 16.f, 8 / 16.f,
  // BACK
  14 / 16.f, 5 / 16.f,
  13 / 16.f, 5 / 16.f,
  13 / 16.f, 8 / 16.f,
  14 / 16.f, 8 / 16.f,
  // LEFT
  11 / 16.f, 5 / 16.f,
  10 / 16.f, 5 / 16.f,
  10 / 16.f, 8 / 16.f,
  11 / 16.f, 8 / 16.f,
};

static const Spline leftArmIdleAnim ({
    {0,    {90.f, 0.f, 0.f}},
    {666,  {90.25f, 0.f, 1.f}},
    {1333, {90.25f, 0.f, -1.f}},
    {2000, {90.f, 0.f, 0.f}},
    {2666, {89.75f, 0.f, 1.f}},
    {3333, {89.75f, 0.f, -1.f}},
    {4000, {90.f, 0.f, 0.f}},

});

static const Spline leftArmWalkAnim ({
    {0,    {135.f, 0.f, 0.0f}},
    {250,  {90.f, 0.f, -1.f}},
    {500,  {45.f, 0.f, 0.0f}},
    {750,  {90.f, 0.f, 1.f}},
    {1000, {135.f, 0.f, 0.0f}},
  
});

static const Spline leftArmDabAnim ({
    {0,  {-10.f, 50.f, 0.f}},
    {50, {-10.f, 50.f, 0.f}},
  
}, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f});

class LeftArm : public Member {

public:
  LeftArm() {
    const glm::mat4 I(1.f);
    
    geometryModel = glm::scale(I, {4, 12, 4});
    geometryModel = glm::translate(I, {0, -4, 0}) * geometryModel;
    //rotate arm by 90 degrees
    glm::quat rot = glm::quat({-glm::pi<float>()/2.f, 0.f, 0.f});
    geometryModel = (glm::mat4)glm::mat4_cast(rot) * geometryModel;

    node.loc = {6, 4, 0};

    anim = std::make_unique<AnimationMixer>(leftArmIdleAnim);
    anim->addAnim(Animation::Walk, leftArmWalkAnim);
    anim->addAnim(Animation::Dab, leftArmDabAnim);
  }


protected:
  std::unique_ptr<Mesh> createMesh() override {
    return std::make_unique<Mesh>(Cube::vertices, Cube::normals, leftArmUVs, Cube::occlusions, Cube::indices, Cube::normalMap);
  }
};