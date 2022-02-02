#include "ShadowMap.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stddef.h>
#include <limits>
#include <vector>

#include "gl/Camera.hpp"
#include "gl/ResourceManager.hpp"
#include "gl/Shader.hpp"

using namespace glm;

ShadowMap::ShadowMap(int size)
  : camera(ivec2(size)),
    shadowMatrices(),
    shader(ResourceManager::getShader("shadow"))
{
  camera.setProjectionType(Projection::PROJECTION_ORTHOGRAPHIC);
  glGenFramebuffers(1, &fbo);
  glGenTextures(3, depthTex);

  for (size_t i = 0; i < 3; i+=1) {
    glBindTexture(GL_TEXTURE_2D, depthTex[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    #ifndef EMSCRIPTEN
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Uncalculated shadows are white
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    #endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  #ifndef EMSCRIPTEN
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  #endif
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::update(vec3 sunDir) {
  camera.setLookAt(vec3(0), sunDir);
}

float linearizeDepth(float depth) { // https://learnopengl.com/Advanced-OpenGL/Depth-testing
    float near = 0.1f;
    float far = 200.0f;
    float z = depth * 2.0f - 1.0f; // back to NDC
    return (2.0f * near * far) / (far + near - z * (far - near)) / far;
}

void ShadowMap::attach(Camera const& cam, Frustum frustum) {

    std::vector<vec3> corners = cam.getBoxCorners(frustum);

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();

    for (auto vec : corners) {
      vec = vec3(camera.view * vec4(vec, 1.0f));
      minX = min(vec.x, minX);
      maxX = max(vec.x, maxX);
      minY = min(vec.y, minY);
      maxY = max(vec.y, maxY);
      minZ = min(-vec.z, minZ);
      maxZ = max(-vec.z, maxZ);
    }

    //render out of the view in case we have to cast shadows from a moutain
    float box[6] = { minX, maxX, minY, maxY, minZ - 10.f * (maxZ - minZ), maxZ };
    camera.setProjectionType(Projection::CUSTOM_PROJECTION, box);
    if(frustum != Frustum::ALL)shadowMatrices[(size_t)frustum] = camera.projection * camera.view;
}

void ShadowMap::beginFrame(Frustum frustum) {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  shader->activate();
  camera.activate();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, getTextureID(frustum), 0);
  glUniformMatrix4fv(shader->getUniform(MATRIX_SHADOWS), 1, GL_FALSE, value_ptr(shadowMatrices[(size_t)frustum]));
  glClear(GL_DEPTH_BUFFER_BIT);
  glDisable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  // glViewport(0, 0, size, size);
}

void ShadowMap::endFrame() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void ShadowMap::activate() {
  Shader* shader = Shader::getActive();
  shader->bindTexture(TEXTURE_SHADOW0, depthTex[0]);
  shader->bindTexture(TEXTURE_SHADOW1, depthTex[1]);
  shader->bindTexture(TEXTURE_SHADOW2, depthTex[2]);
  glUniformMatrix4fv(shader->getUniform(MATRIX_SHADOWS), 3, GL_FALSE, (GLfloat*)shadowMatrices);
}

#include "debug/Debug.hpp"

ShadowMap::~ShadowMap() {
  ASSERT_GL_MAIN_THREAD();
  glDeleteTextures(3, depthTex);
  glDeleteFramebuffers(1, &fbo);
}

GLuint ShadowMap::getTextureID(Frustum frustum) const {
  return depthTex[(size_t)frustum];
}
