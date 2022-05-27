#pragma once

#include <vector>
#include <GL/glew.h>


namespace Cube {

    static const std::vector<GLfloat> vertices = {
      // TOP
      -0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      // BOTTOM
      0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      // FRONT
      0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      // RIGHT
      0.5f,  0.5f, -0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f, -0.5f,
      // BACK
      -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      // LEFT
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
    };

    static const std::vector<GLfloat> normals = {
      // TOP
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      // BOTTOM
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      // FRONT
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      // RIGHT
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      // BACK
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      // LEFT
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
    };

    static const std::vector<GLfloat> occlusions = {
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
      0.f, 0.f, 0.f, 0.f,
    };

    static const std::vector<GLuint> indices = {
      0, 1, 2, 0, 2, 3,
      4, 5, 6, 4, 6, 7,
      8, 9, 10, 8, 10, 11,
      12, 13, 14, 12, 14, 15,
      16, 17, 18, 16, 18, 19,
      20, 21, 22, 20, 22, 23,
    };

//TODO
    static const std::vector<GLfloat> normalMap = {
  //TOP
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  // BOTTOM
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  // FRONT
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  // RIGHT
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  // BACK
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  // LEFT
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, -1.0f,
    };
};
