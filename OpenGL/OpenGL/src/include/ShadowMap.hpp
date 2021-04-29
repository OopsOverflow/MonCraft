#pragma once

#include "Shader.hpp"
#include "Camera.hpp"

class ShadowMap {

public:

  ShadowMap();
  ~ShadowMap();

  ShadowMap(ShadowMap const&) = delete;
  ShadowMap& operator=(ShadowMap&) = delete;


  void changeDirection(glm::vec3 direction);
  void update(Camera& cam, Frustrum frustrum);
  void beginFrame();
  void endFrame();
  void activate(Frustrum frustrum);
  GLuint getTextureID(Frustrum frustrum) const;
  Camera camera;
private:
  GLuint fbo;
  GLuint depthTex[3];
  Shader shader;
  float distance;

  glm::vec3 direction;
};
