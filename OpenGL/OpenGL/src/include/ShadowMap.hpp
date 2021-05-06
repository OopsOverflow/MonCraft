#pragma once

#include "Shader.hpp"
#include "Camera.hpp"

class ShadowMap {

public:

  ShadowMap(float size);
  ~ShadowMap();

  ShadowMap(ShadowMap const&) = delete;
  ShadowMap& operator=(ShadowMap&) = delete;


  void changeDirection(glm::vec3 direction);
  void update(Camera& cam, Frustrum frustrum);
  void beginFrame();
  void endFrame();
  void activate(Shader& shader);
  void bindForWriting(Frustrum frustrum);
  GLuint getTextureID(Frustrum frustrum) const;
  Camera camera;
private:
  GLuint fbo;
  GLuint depthTex[3];
  Shader shader;
  float distance;

  glm::mat4 shadowMatrix[3];
  glm::vec3 direction;
};
