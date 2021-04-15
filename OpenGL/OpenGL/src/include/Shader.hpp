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
  MATRIX_MODEL_VIEW = 0,
  MATRIX_PROJECTION = 1,
  MATRIX_NORMAL = 2,
  
  CAMERA_CENTER = -1,
};

class Shader {
public:
  Shader(const char *vertexPath, const char *fragmentPath);

  void activate();
  GLint getUniformLocation(const std::string &location);
  static Shader *getActive();

  GLuint program;

private:
  static Shader *activeShader;

  void initLocations();

  std::string readFile(const std::string &path);
  GLuint compileShader(int type, const std::string &shader);
  GLuint createShader(int type, const char *path);
  GLuint createProgram(GLuint vertex, GLuint fragment);
};
