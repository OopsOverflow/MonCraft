#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>

enum ShaderLocation {
  VERTEX_POSITION = 0,
  VERTEX_NORMAL = 1,
  VERTEX_TEXTURE = 2,
  MATRIX_MODEL = 0,
  MATRIX_VIEW = 1,
  MATRIX_PROJECTION = 2,
  MATRIX_NORMAL = 3,
  MATRIX_SHADOWS = 4,

  CAMERA_CENTER = -1,
  VERTEX_OCCLUSION = 3,
};

class Shader {
public:
  Shader(const char *vertexPath, const char *fragmentPath);

  void activate();
  GLint getUniformLocation(const std::string &location) const;
  static Shader *getActive();
  GLuint getLocation(ShaderLocation location) const;
  GLuint program;

private:
  static Shader *activeShader;


  void initLocations();

  std::string readFile(const std::string &path);
  GLuint compileShader(int type, const std::string &shader);
  GLuint createShader(int type, const char *path);
  GLuint createProgram(GLuint vertex, GLuint fragment);
};
