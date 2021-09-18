#include "ShadowMap.hpp"
#include "ResourceManager.hpp"
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

ShadowMap::ShadowMap(int size)
  : camera(ivec2(size), {10, 10, 10}, {0, 0, 0}, Projection::PROJECTION_ORTHOGRAPHIC),
    shader(ResourceManager::getShader("shadow")),
    distance(100.f),
    size(size)
{
  glGenFramebuffers(1, &fbo);
  glGenTextures(3, depthTex);

  for (size_t i = 0; i < 3; i+=1) {
    glBindTexture(GL_TEXTURE_2D, depthTex[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
    float near = 0.1;
    float far = 200.0;
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near)) / far;
}

void ShadowMap::attach(Camera const& cam, Frustum frustum) {

    std::vector<vec3> corners = cam.getBoxCorners(frustum);

    float minX = FLT_MAX;
    float maxX = -FLT_MAX;
    float minY = FLT_MAX;
    float maxY = -FLT_MAX;
    float minZ = FLT_MAX;
    float maxZ = -FLT_MAX;

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
    float box[6] = { minX, maxX, minY, maxY, 5 * minZ - 4 * maxZ, maxZ };
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
