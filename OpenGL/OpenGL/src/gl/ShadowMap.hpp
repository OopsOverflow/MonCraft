#pragma once

#include "../gl/Shader.hpp"
#include "../gl/Camera.hpp"

class ShadowMap {

public:

  ShadowMap(int size);
  ~ShadowMap();

  ShadowMap(ShadowMap const&) = delete;
  ShadowMap& operator=(ShadowMap&) = delete;

  void update(glm::vec3 sunPos, glm::vec3 center);
  void attach(Camera const& cam, Frustum frustum);
  void beginFrame(Frustum frustum);
  void endFrame();
  void activate();
  GLuint getTextureID(Frustum frustum) const;

  Camera camera;
private:
  GLuint fbo;
  GLuint depthTex[3];
  int size;
  Shader shader;
  float distance;
  glm::mat4 shadowMatrices[3];
};
