#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>

enum ShaderLocation {
  VERTEX_POSITION,
  VERTEX_COLOR,
  VERTEX_NORMAL,
  VERTEX_TEXTURE,
  MATRIX_MODEL_VIEW,
  MATRIX_PROJECTION,
  MATRIX_NORMAL,
  CAMERA_CENTER,
};

class Shader {
public:
  Shader(const char *vertexPath, const char *fragmentPath);

  void activate();
  GLint getUniformLocation(const std::string &location);
  GLuint getLocation(ShaderLocation location) const;
  static Shader *getActive();

  GLuint program;

private:
  static Shader *activeShader;

  GLint locations[8];

  void initLocations();

  std::string readFile(const std::string &path);
  GLuint compileShader(int type, const std::string &shader);
  GLuint createShader(int type, const char *path);
  GLuint createProgram(GLuint vertex, GLuint fragment);
};
