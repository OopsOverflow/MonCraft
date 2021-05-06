#include "ShadowMap.hpp"
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

ShadowMap::ShadowMap(float resolution)
  : resolution(resolution),
    camera(10, 10, {10, 10, 10}, {0, 0, 0}, Projection::PROJECTION_ORTHOGRAPHIC),
    shader("src/shader/shadow.vert", "src/shader/shadow.frag"),
    distance(100.f)
{
  glGenFramebuffers(1, &fbo);
  glGenTextures(3, depthTex);

  for (size_t i = 0; i < 3; i+=1) {
    glBindTexture(GL_TEXTURE_2D, depthTex[i]);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 10, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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

void ShadowMap::update(vec3 sunPos, vec3 center) {
  camera.setLookAt(sunPos, center);
}

void ShadowMap::attach(Camera const& cam) {

  unsigned int newW, newH;
  cam.getSize(newW, newH);
  if(newW != width || newH != height) {
    width = newW; height = newH;
    for (size_t i = 0; i < 3; i+=1) {
      glBindTexture(GL_TEXTURE_2D, depthTex[i]);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, newW * resolution, newH * resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    camera.setSize(newW * resolution, newH * resolution);
  }

  std::vector<vec3> corners[] = {
    cam.getBoxCorners(Frustum::NEAR),
    cam.getBoxCorners(Frustum::MEDIUM),
    cam.getBoxCorners(Frustum::FAR),
  };

  for (size_t i = 0; i < 3; i++) {
    float minX = FLT_MAX;
    float maxX = -FLT_MAX;
    float minY = FLT_MAX;
    float maxY = -FLT_MAX;
    float minZ = FLT_MAX;
    float maxZ = -FLT_MAX;

    for (auto vec : corners[i]) {
      vec = vec3(camera.view * vec4(vec, 1.0f));
      minX = min(vec.x, minX);
      maxX = max(vec.x, maxX);
      minY = min(vec.y, minY);
      maxY = max(vec.y, maxY);
      minZ = min(-vec.z, minZ);
      maxZ = max(-vec.z, maxZ);
    }

    //render out of the view in case we have to cast shadows from a moutain
    float box[6] = { minX,maxX,minY,maxY,2 * minZ - maxZ,maxZ };
    camera.setProjectionType(Projection::CUSTOM_PROJECTION, box);
    shadowMatrices[i] = camera.projection * camera.view;
  }
}

void ShadowMap::beginFrame(Frustum frustum) {
  shader.activate();
  camera.activate();
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, getTextureID(frustum), 0);
  glUniformMatrix4fv(MATRIX_SHADOWS, 1, GL_FALSE, value_ptr(shadowMatrices[(size_t)frustum]));
  glClear(GL_DEPTH_BUFFER_BIT);
  glDisable(GL_CULL_FACE);
  glViewport(0, 0, width * resolution, height * resolution);
}

void ShadowMap::endFrame() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glEnable(GL_CULL_FACE);
}

void ShadowMap::activate(Shader const& shader) {
  GLint shadowSampler = shader.getUniformLocation("shadowSampler[0]");
  glUniform1i(shadowSampler, 1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, depthTex[0]);

  shadowSampler = shader.getUniformLocation("shadowSampler[1]");
  glUniform1i(shadowSampler, 2);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, depthTex[1]);

  shadowSampler = shader.getUniformLocation("shadowSampler[2]");
  glUniform1i(shadowSampler, 3);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, depthTex[2]);

  glUniformMatrix4fv(MATRIX_SHADOWS, 3, GL_FALSE, (GLfloat*)shadowMatrices);
}

ShadowMap::~ShadowMap() {
  glDeleteTextures(3, depthTex);
  glDeleteFramebuffers(1, &fbo);
}

GLuint ShadowMap::getTextureID(Frustum frustum) const {
  return depthTex[(size_t)frustum];
}
