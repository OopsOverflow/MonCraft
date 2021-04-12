#include "Camera.hpp"
#include "Shader.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera(unsigned int width, unsigned int height, const glm::vec3 &position,
               const glm::vec3 &center, Projection projType):
 position(position), center(center), fovY(45.f), near_(0.1f), far_(100.f),
 screenWidth(width), screenHeight(height), projType(projType)
{
  computeView();
  computeProjection();
}

void Camera::activate() {
  glViewport(0, 0, screenWidth, screenHeight);
  glm::mat4 normal = glm::transpose(glm::inverse(view));
  Shader *shader = Shader::getActive();
  if (shader) {
    glUniformMatrix4fv(shader->getLocation(MATRIX_MODEL_VIEW), 1, GL_FALSE,
                       glm::value_ptr(view));
    glUniformMatrix4fv(shader->getLocation(MATRIX_NORMAL), 1, GL_FALSE,
                       glm::value_ptr(normal));
    glUniformMatrix4fv(shader->getLocation(MATRIX_PROJECTION), 1, GL_FALSE,
                       glm::value_ptr(projection));
    glm::vec3 c = view * glm::vec4(center, 1.f);
    glUniform3f(shader->getLocation(CAMERA_CENTER), c.x, c.y, c.z);
  } else {
    std::cout << "error: camera activated but no shader bound" << std::endl;
  }
}

void Camera::setPosition(const glm::vec3 &newPos) {
  view = glm::translate(view, -(newPos - position));
  position = newPos;
}

void Camera::translate(const glm::vec3 &translation, bool localSpace) {
  glm::mat4 trans = glm::translate(glm::mat4(1.f), translation);
  glm::mat4 invTrans = glm::translate(glm::mat4(1.f), -translation);

  if (localSpace) {
    position = glm::inverse(view) * trans * view * glm::vec4(position, 1.f);
    center = glm::inverse(view) * trans * view * glm::vec4(center, 1.f);
    view = invTrans * view;
  }
  else {
    view = view * invTrans;
    position += translation;
    center += translation;
  }
}

void Camera::rotate(const glm::vec3 &rotation, bool localSpace) {
  glm::mat4 rot(1.f);
  rot = glm::rotate(rot, glm::radians(rotation.x), {1, 0, 0});
  rot = glm::rotate(rot, glm::radians(rotation.y), {0, 1, 0});
  rot = glm::rotate(rot, glm::radians(rotation.z), {0, 0, 1});

  if (localSpace) {
    center = glm::inverse(view) * rot * view * glm::vec4(center, 1.f);
    view = glm::inverse(rot) * view;
  } else {
    glm::mat4 trans = glm::translate(glm::mat4(1.f), center);
    position = trans * rot * glm::inverse(trans) * glm::vec4(position, 1.f);
    view = view * glm::inverse(trans) * glm::inverse(rot) * trans;
  }

  // rotation can accumulate imperfections - better to recalculate.
  computeView();
}

void Camera::setSize(unsigned int width, unsigned int height) {
  screenWidth = width;
  screenHeight = height;
  computeProjection();
}

void Camera::setProjectionType(Projection projType) {
  this->projType = projType;
  computeProjection();
}

void Camera::translatePixels(int x, int y) {
  glm::vec3 translation((float)x, (float)-y, 0.f);

  float aspect = (float)screenWidth / (float)screenHeight;
  float localHeight =
      2.f * glm::length(center - position) * tan(glm::radians(fovY / 2.f));
  float localWidth = localHeight * aspect;

  translation.x *= localWidth / (float)screenWidth;
  translation.y *= localHeight / (float)screenHeight;

  translate(translation, true);
}

void Camera::zoom(float factor) {
  glm::vec3 translation = center - position;
  float coef = 1.f - 1.f / (float)pow(2, factor);
  translation *= coef;

  glm::mat4 trans(1.f);
  trans = glm::translate(trans, -translation);

  position += translation;
  view = view * trans;

  if (projType == Projection::PROJECTION_ORTHOGRAPHIC) {
    computeProjection();
  }
}

void Camera::rotatePixels(int x, int y) {
  // in the turnTable rotation style we rotate around y axis in global space
  // and around x axis in local space.

  float rotX, rotY;

  // can rotate by maxRotation degrees around x axis while translating from 0
  // to screenWidth (resp. y axis & screenHeight).
  float maxRotation = 360.f;

  rotY = x * maxRotation / (float)screenWidth;
  rotX = y * maxRotation / (float)screenHeight;

  rotate({0.f, rotY, 0.f});
  // rotAxis should be normalized already (translations and rotations)
  glm::vec3 rotAxis = glm::inverse(view) * glm::vec4(1.f, 0.f, 0.f, 0.f);
  rotate(rotX * rotAxis);
}

// ----------- getters -----------

void Camera::getSize(unsigned int &width, unsigned int &height) const {
  width = screenWidth;
  height = screenHeight;
}

Projection Camera::getProjectionType() const { return projType; }

glm::mat4 Camera::getView() const { return view; }

// ----------- private -----------

void Camera::computeView() {
  glm::vec3 up(0.f, 1.f, 0.f);
  glm::vec3 cameraUp = view * glm::vec4(up, 0.0);
  if (glm::dot(up, cameraUp) < 0) {
    up = -up;
  }
  view = glm::lookAt(position, center, up);
}

void Camera::computeProjection() {
  float aspect = (float)screenWidth / (float)screenHeight;

  if (projType == Projection::PROJECTION_ORTHOGRAPHIC) {
    // kind of perspective division... To switch between persp & ortho.
    float y = glm::length(center - position) * tan(glm::radians(fovY / 2.f));
    float x = y * aspect;
    projection = glm::ortho(-x, x, -y, y, -1000.f, 1000.f);
  }

  else if (projType == Projection::PROJECTION_PERSPECTIVE) {
    // COMBAK: Here I am using a far plane at infinity to make sure the
    // whole scene fits. One can argue that the projection looses precision but
    // I have never seen any artefacts so far. Anyway I haven't understood the
    // precision issues with matrices.
    projection = glm::infinitePerspective(glm::radians(fovY), aspect, near_);
  }
}
