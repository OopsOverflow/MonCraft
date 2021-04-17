#include "ShadowMap.hpp"
#include <glm/gtc/type_ptr.hpp>

using glm::vec3;

ShadowMap::ShadowMap(int size)
  : size(size),
    camera(size, size, {10, 10, 10}, {0, 0, 0}, Projection::PROJECTION_ORTHOGRAPHIC),
    shader("src/shader/shadow.vert", "src/shader/shadow.frag"),
    distance(100.f)
{
  glGenFramebuffers(1, &fbo);
  glGenTextures(1, &tex);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_TEXTURE_2D, 0);
}

void ShadowMap::update(glm::vec3 sunPos, glm::vec3 center) {
  camera.setLookAt(sunPos, center);
}

void ShadowMap::beginFrame() {
  shader.activate();
  camera.activate();
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_DEPTH_BUFFER_BIT);
  auto shadows = camera.projection * camera.view;
  glUniformMatrix4fv(MATRIX_SHADOWS, 1, GL_FALSE, glm::value_ptr(shadows));
  // glCullFace(GL_FRONT);
}

void ShadowMap::endFrame() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // glCullFace(GL_BACK);
}

void ShadowMap::activate() {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, tex);
  auto shadows = camera.projection * camera.view;
  glUniformMatrix4fv(MATRIX_SHADOWS, 1, GL_FALSE, glm::value_ptr(shadows));
}

ShadowMap::~ShadowMap() {
  glDeleteTextures(1, &tex);
  glDeleteFramebuffers(1, &fbo);
}

GLuint ShadowMap::getTextureID() const {
  return tex;
}
