#pragma once

#include "../gl/Shader.hpp"
#include "../gl/Camera.hpp"

class ShadowMap {

public:

  ShadowMap(int size);
  ~ShadowMap();

  ShadowMap(ShadowMap const&) = delete;
  ShadowMap& operator=(ShadowMap&) = delete;

  void update(glm::vec3 sunDir);
  void attach(Camera const& cam, Frustum frustum);
  void beginFrame(Frustum frustum);
  void endFrame();
  void activate(Shader const& shader);
  GLuint getTextureID(Frustum frustum) const;

  Camera camera;
  glm::mat4 shadowMatrices[3];
private:
  GLuint fbo;
  GLuint depthTex[3];
  float resolution;
  Shader shader;
  float distance;
  int size;
};
