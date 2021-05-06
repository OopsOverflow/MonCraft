#include "ShadowMap.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>


ShadowMap::ShadowMap(float size)
  :camera(size, size, {10, 10, 10}, {0, 0, 0}, Projection::PROJECTION_ORTHOGRAPHIC),
    shader("src/shader/shadow.vert", "src/shader/shadow.frag"),
    distance(100.f), direction({ -10, -10, -10 })
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

void ShadowMap::changeDirection(glm::vec3 direction) {
    this->direction = direction;
    camera.setLookAt({ 0.0f,0.0f,0.0f }, direction);
}

void ShadowMap::update(Camera& cam, Frustrum frustrum) {
    float minX = FLT_MAX;
    float maxX = -FLT_MAX;
    float minY = FLT_MAX;
    float maxY = -FLT_MAX;
    float minZ = FLT_MAX;
    float maxZ = -FLT_MAX;

    std::vector<glm::vec3> corners = cam.getBoxCorners(frustrum);
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
    camera.activate();
    shadowMatrix[(size_t)frustrum] = camera.projection * camera.view;

    camera.setProjectionType(Projection::PROJECTION_ORTHOGRAPHIC);
    camera.setLookAt({0, 50, 0}, {0, 0, 1});
}

void ShadowMap::beginFrame() {
  shader.activate();
  camera = Camera(1024, 1024, {0, 50, 0}, {0, 0, 1});
  camera.activate();
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_DEPTH_BUFFER_BIT);
  auto shadows = camera.projection * camera.view;
  glUniformMatrix4fv(MATRIX_SHADOWS, 1, GL_FALSE, glm::value_ptr(shadows));
}

void ShadowMap::endFrame() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::activate(Shader& shader) {
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

  glUniformMatrix4fv(MATRIX_SHADOWS, 3, GL_FALSE, (GLfloat*) shadowMatrix);
}

void ShadowMap::bindForWriting(Frustrum frustrum) {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, getTextureID(frustrum), 0);
}


ShadowMap::~ShadowMap() {
  glDeleteTextures(3, depthTex);
  glDeleteFramebuffers(1, &fbo);
}

GLuint ShadowMap::getTextureID(Frustrum frustrum) const {
    return depthTex[(size_t)frustrum];
}
