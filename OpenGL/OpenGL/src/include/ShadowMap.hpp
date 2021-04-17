#pragma once

#include "Shader.hpp"
#include "Camera.hpp"

class ShadowMap {

public:

  ShadowMap(int size);
  ~ShadowMap();

  ShadowMap(ShadowMap const&) = delete;
  ShadowMap& operator=(ShadowMap&) = delete;

  void update(glm::vec3 sunPos, glm::vec3 center);
  void beginFrame();
  void endFrame();
  void activate();
  GLuint getTextureID() const;

private:
  GLuint fbo;
  GLuint tex;
  int size;
  Camera camera;
  Shader shader;
  float distance;
};
