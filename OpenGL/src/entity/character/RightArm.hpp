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
  {0,    {0.f, -1.f, 0.f}},
  {666,  {-0.05f, -1.f, 0.005f}},
  {1333, {0.05f, -1.f, 0.005f}},
  {2000, {0.f, -1.f, 0.f}},
  {2666, {-0.05f, -1.f, -0.005f}},
  {3333, {0.05f, -1.f, -0.005f}},
  {4000, {0.f, -1.f, 0.f}},

});

static const Spline rightArmWalkAnim ({
  {0,    {0.f, -1.f, 1.0f}},
  {250,  {0.05f, -1.f, 0.f}},
  {500,  {0.f, -1.f, -1.0f}},
  {750,  {-0.05f, -1.f, 0.f}},
  {1000, {0.f, -1.f, 1.0f}},
  
});

static const Spline rightArmBreakAnim ({
  {0,   {-0.50f, -0.25f, 0.80f}},
  {50,  {-0.65f, 0.05f, 0.80f}},
  {100,  {0.15f, 0.25f, 1.f}},
  {150, {0.15f, -0.25f, 1.f}},
  
}, {0.f, smoothing, 0.f}, {0.f, -smoothing, 0.f});

static const Spline rightArmDabAnim ({
  {0,  {.4f, 0.3f, 0.7f}},
  {50, {.4f, 0.3f, 0.7f}},
  
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
