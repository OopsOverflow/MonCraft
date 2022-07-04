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

static const Spline rightArmIdleAnim ({
  {0,    {90.f, 0.f, 0.f}},
  {666,  {89.75f, 0.f, -1.f}},
  {1333, {89.75f, 0.f, 1.f}},
  {2000, {90.f, 0.f, 0.f}},
  {2666, {90.25f, 0.f, -1.f}},
  {3333, {90.25f, 0.f, 1.f}},
  {4000, {90.f, 0.f, 0.f}},

});

static const Spline rightArmWalkAnim ({
  {0,    {45.f, 0.f, 0.0f}},
  {250,  {90.f, 0.f, 1.f}},
  {500,  {135.f, 0.f, 0.0f}},
  {750,  {90.f, 0.f, -1.f}},
  {1000, {45.f, 0.f, 0.0f}},
  
});

static const Spline rightArmBreakAnim ({
  {0,   {10.f, -35.f, 0.f}},
  {50,  {-20.f, -25.f, 0.f}},
  {100, {-20.f, -0.f, 0.f}},
  {150, {-5.f, 10.f, 0.f}},
  
}, glm::normalize(glm::vec3(10.0f, -35.f, 0.f)) * smoothing, glm::normalize(glm::vec3(-20.f, 0.f, 0.f)) * smoothing);

static const Spline rightArmDabAnim ({
  {0,  {-5.f, 35.f, 40.f}},
  {50, {-5.f, 35.f, 40.f}},
  
}, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f});

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

    anim = std::make_unique<AnimationMixer>(rightArmIdleAnim);
    anim->addAnim(Animation::Walk, rightArmWalkAnim);
    anim->addAnim(Animation::Break, rightArmBreakAnim);
    anim->addAnim(Animation::Dab, rightArmDabAnim);
  }

protected:
  std::unique_ptr<Mesh> createMesh() override {
    return std::make_unique<Mesh>(Cube::vertices, Cube::normals, rightArmUVs, Cube::occlusions, Cube::indices, Cube::normalMap);
  }
};
