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
  glGenTextures(3, depthTex);

  for (size_t i = 0; i < 3; i+=1) {
    glBindTexture(GL_TEXTURE_2D, depthTex[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };//Uncalculated shadows are white
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// TODO: temporary until I finish the skewed shadow matrix
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform2.hpp>

void ShadowMap::update(glm::vec3 sunPos, glm::vec3 center) {
  camera.setLookAt(sunPos, center);
}

void ShadowMap::attach(Camera const& cam, Frustum frustum) {
  float minX = FLT_MAX;
  float maxX = -FLT_MAX;
  float minY = FLT_MAX;
  float maxY = -FLT_MAX;
  float minZ = FLT_MAX;
  float maxZ = -FLT_MAX;

  std::vector<glm::vec3> corners = cam.getBoxCorners(frustum);
  for (glm::vec3 vec : corners)
  {
      vec = glm::vec3(camera.view * glm::vec4(vec, 1.0f));
      minX = glm::min(vec.x, minX);
      maxX = glm::max(vec.x, maxX);
      minY = glm::min(vec.y, minY);
      maxY = glm::max(vec.y, maxY);
      minZ = glm::min(-vec.z, minZ);
      maxZ = glm::max(-vec.z, maxZ);
  }

  //render out of the view in case we have to cast shadows from a moutain
  float box[6] = { minX,maxX,minY,maxY,2 * minZ - maxZ,maxZ };
  camera.setProjectionType(Projection::CUSTOM_PROJECTION, box);
}

void ShadowMap::beginFrame(Frustum frustum) {
  shader.activate();
  camera.activate();
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_DEPTH_BUFFER_BIT);
  auto shadows = camera.projection * camera.view;
  glUniformMatrix4fv(MATRIX_SHADOWS, 1, GL_FALSE, glm::value_ptr(shadows));

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, getTextureID(frustum), 0);
}

void ShadowMap::endFrame() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::activate() {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, depthTex[0]);
  auto shadows = camera.projection * camera.view;
  glUniformMatrix4fv(MATRIX_SHADOWS, 1, GL_FALSE, glm::value_ptr(shadows));
}

ShadowMap::~ShadowMap() {
  glDeleteTextures(3, depthTex);
  glDeleteFramebuffers(1, &fbo);
}

GLuint ShadowMap::getTextureID(Frustum frustum) const {
  return depthTex[(size_t)frustum];
}
