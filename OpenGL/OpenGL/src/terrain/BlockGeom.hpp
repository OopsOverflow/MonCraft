#pragma once

#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>

enum class BlockFace { TOP, BOTTOM, FRONT, RIGHT, BACK, LEFT };

template<size_t N>
using face_t = std::array<GLfloat, 4 * N>;

template<size_t N>
using BlockData = std::array<face_t<N>, 6>;


static const int STACKED_TEXTURES = 2; // Can stack texture on a single image to load less images
static const int textureNumber = 0;    // Texture Id for future implementation
static const float botTexture = (float)(textureNumber + 1) / STACKED_TEXTURES;
static const float topTexture = (float)(textureNumber + 0) / STACKED_TEXTURES;

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

static const BlockData<2> blockUVs {
  face_t<2>{ // TOP
    1.f/6, topTexture,
    0.f/6, topTexture,
    0.f/6, botTexture,
    1.f/6, botTexture,
  }, { // BOTTOM
    2.f/6, topTexture,
    1.f/6, topTexture,
    1.f/6, botTexture,
    2.f/6, botTexture,
  }, { // FRONT
    3.f/6, topTexture,
    2.f/6, topTexture,
    2.f/6, botTexture,
    3.f/6, botTexture,
  }, { // RIGHT
    4.f/6, topTexture,
    3.f/6, topTexture,
    3.f/6, botTexture,
    4.f/6, botTexture,
  }, { // BACK
    5.f/6, topTexture,
    4.f/6, topTexture,
    4.f/6, botTexture,
    5.f/6, botTexture,
  }, { // LEFT
    6.f/6, topTexture,
    5.f/6, topTexture,
    5.f/6, botTexture,
    6.f/6, botTexture,
  }
};
