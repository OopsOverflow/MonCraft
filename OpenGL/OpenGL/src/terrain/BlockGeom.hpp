#pragma once

#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../blocks/Block.h"

template<size_t N>
using face_t = std::array<GLfloat, 4 * N>;

template<size_t N>
using BlockData = std::array<face_t<N>, 6>;

static const BlockData<3> blockPositions = {
  face_t<3>{ // TOP
    -0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
  }, { // BOTTOM
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
  }, { // FRONT
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
  }, { // RIGHT
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
  }, { // BACK
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
  }, { // LEFT
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
  }
};

static const BlockData<3> blockNormals {
  face_t<3>{ // TOP
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
  }, { // BOTTOM
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
  }, { // FRONT
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
  }, { // RIGHT
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
  }, { // BACK
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
  }, { // LEFT
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
  }
};

static const std::array<std::array<glm::ivec3, 8>, 6> blockOcclusionOffsets = {
  std::array<glm::ivec3, 8> { // TOP
    glm::ivec3{-1,  1,  0 },
    glm::ivec3{-1,  1,  1 },
    glm::ivec3{ 0,  1,  1 },
    glm::ivec3{ 1,  1,  1 },
    glm::ivec3{ 1,  1,  0 },
    glm::ivec3{ 1,  1, -1 },
    glm::ivec3{ 0,  1, -1 },
    glm::ivec3{-1,  1, -1 },
  },
  std::array<glm::ivec3, 8> { // BOTTOM
    glm::ivec3{ 1, -1,  0 },
    glm::ivec3{ 1, -1,  1 },
    glm::ivec3{ 0, -1,  1 },
    glm::ivec3{-1, -1,  1 },
    glm::ivec3{-1, -1,  0 },
    glm::ivec3{-1, -1, -1 },
    glm::ivec3{ 0, -1, -1 },
    glm::ivec3{ 1, -1, -1 },
  },
  std::array<glm::ivec3, 8> { // FRONT
    glm::ivec3{ 1,  0,  1 },
    glm::ivec3{ 1,  1,  1 },
    glm::ivec3{ 0,  1,  1 },
    glm::ivec3{-1,  1,  1 },
    glm::ivec3{-1,  0,  1 },
    glm::ivec3{-1, -1,  1 },
    glm::ivec3{ 0, -1,  1 },
    glm::ivec3{ 1, -1,  1 },
  },
  std::array<glm::ivec3, 8> { // RIGHT
    glm::ivec3{ 1,  0, -1 },
    glm::ivec3{ 1,  1, -1 },
    glm::ivec3{ 1,  1,  0 },
    glm::ivec3{ 1,  1,  1 },
    glm::ivec3{ 1,  0,  1 },
    glm::ivec3{ 1, -1,  1 },
    glm::ivec3{ 1, -1,  0 },
    glm::ivec3{ 1, -1, -1 },
  },
  std::array<glm::ivec3, 8> { // BACK
    glm::ivec3{-1,  0, -1 },
    glm::ivec3{-1,  1, -1 },
    glm::ivec3{ 0,  1, -1 },
    glm::ivec3{ 1,  1, -1 },
    glm::ivec3{ 1,  0, -1 },
    glm::ivec3{ 1, -1, -1 },
    glm::ivec3{ 0, -1, -1 },
    glm::ivec3{-1, -1, -1 },
  },
  std::array<glm::ivec3, 8> { // LEFT
    glm::ivec3{-1,  0,  1 },
    glm::ivec3{-1,  1,  1 },
    glm::ivec3{-1,  1,  0 },
    glm::ivec3{-1,  1, -1 },
    glm::ivec3{-1,  0, -1 },
    glm::ivec3{-1, -1, -1 },
    glm::ivec3{-1, -1,  0 },
    glm::ivec3{-1, -1,  1 },
  },
};
