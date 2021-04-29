#include "ShadowMap.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>


ShadowMap::ShadowMap()
  : size(size),
    camera(size, size, {10, 10, 10}, {0, 0, 0}, Projection::PROJECTION_ORTHOGRAPHIC),
    shader("src/shader/shadow.vert", "src/shader/shadow.frag"),
    distance(100.f), direction({ -10, -10, -10 })
{
  glGenFramebuffers(1, &fbo);
  glGenTextures(3, depthTex);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  for (int i = 0; i < 3; i++) {
      glBindTexture(GL_TEXTURE_2D, depthTex[i]);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 512, 512, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };//Uncalculated shadows are white
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex[0], 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_TEXTURE_2D, 0);
}

// TODO: temporary until I finish the skewed shadow matrix
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform2.hpp>


void ShadowMap::changeDirection(glm::vec3 direction) {
    this->direction = direction;

}

void ShadowMap::update(Camera& cam) {
    float minX = FLT_MAX;
    float maxX = -FLT_MAX;
    float minY = FLT_MAX;
    float maxY = -FLT_MAX;
    float minZ = FLT_MAX;
    float maxZ = -FLT_MAX;

    std::vector<glm::vec3> corners = cam.getBoxCorners(Frustrum::NEAR);
    for (glm::vec3 vec : corners)
    {
        vec = glm::vec3(camera.view * glm::vec4(vec, 1.0f));
        minX = glm::min(vec.x, minX);
        maxX = glm::max(vec.x, maxX);
        minY = glm::min(vec.y, minY);
        maxY = glm::max(vec.y, maxY);
        minZ = glm::min(vec.z, minZ);
        maxZ = glm::max(vec.z, maxZ);

    }
    glm::vec4 minPoint = glm::inverse(camera.view) * glm::vec4(minX, minY, minZ, 1.0f);
    glm::vec4 maxPoint = glm::inverse(camera.view) * glm::vec4(maxX, maxY, maxZ, 1.0f);
    glm::vec3 center = { (minPoint.x + maxPoint.x) * 0.5,(minPoint.y + maxPoint.y) * 0.5, (minPoint.z + maxPoint.z) * 0.5 };
    float xSize = abs(maxPoint.x - minPoint.x);
    float ySize = abs(maxPoint.y - minPoint.y);
    if (xSize > ySize) {
        camera.setSize(1024 * xSize / ySize, 1024);
    }
    else {
        camera.setSize(1024, 1024 * ySize / xSize);
    }
    camera.setLookAt(center - direction, center);




  //auto vecX4 = proj * view * glm::vec4(1, 0, 0, 0);
  //auto vecY4 = proj * view * glm::vec4(0, 1, 0, 0);
  //auto vecZ4 = proj * view * glm::vec4(0, 0,-1, 0);
  //auto vecX = glm::normalize(glm::vec2(vecX4.x, vecX4.y));
  //auto vecY = glm::normalize(glm::vec2(vecY4.x, vecY4.y));
  //auto vecZ = glm::normalize(glm::vec2(vecZ4.x, vecZ4.y));

  //auto angleA = glm::orientedAngle(vecX, vecY);

  //auto shear1 = glm::shearX3D(glm::mat4(1.f), -(float)tan(glm::half_pi<float>() - angleA), 0.f);
  //proj = shear1 * proj;

  //vecX4 = proj * view * glm::vec4(1, 0, 0, 0);
  //vecY4 = proj * view * glm::vec4(0, 1, 0, 0);
  //vecZ4 = proj * view * glm::vec4(0, 0,-1, 0);
  //vecX = glm::normalize(glm::vec2(vecX4.x, vecX4.y));
  //vecY = glm::normalize(glm::vec2(vecY4.x, vecY4.y));
  //vecZ = glm::normalize(glm::vec2(vecZ4.x, vecZ4.y));
  //auto angleB = glm::orientedAngle(vecZ, vecY);

  //auto shear2 = glm::shearY3D(glm::mat4(1.f), -(float)tan(angleB), 0.f);
  //proj = shear2 * proj;

  //camera.projection = proj;
}




void ShadowMap::beginFrame() {
  shader.activate();
  camera.activate();
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_DEPTH_BUFFER_BIT);
  auto shadows = camera.projection * camera.view;
  glUniformMatrix4fv(MATRIX_SHADOWS, 1, GL_FALSE, glm::value_ptr(shadows));
  //glCullFace(GL_FRONT);
}

void ShadowMap::endFrame() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //glCullFace(GL_BACK);
}

void ShadowMap::activate(Frustrum frustrum) {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, depthTex[0]);
  auto shadows = camera.projection * camera.view;
  glUniformMatrix4fv(MATRIX_SHADOWS, 1, GL_FALSE, glm::value_ptr(shadows));
}

ShadowMap::~ShadowMap() {
  glDeleteTextures(1, depthTex);
  glDeleteFramebuffers(1, &fbo);
}

GLuint ShadowMap::getTextureID(Frustrum frustrum) const {
  return depthTex[0];
}
