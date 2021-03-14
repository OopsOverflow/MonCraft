#include "controller/Turntable.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

ControllerTurntable::ControllerTurntable() {
  lastX = lastY = 0;
  translation = false;
  rotation = false;
  zoomFactor = 0;
}

void ControllerTurntable::translateStart(int x, int y) {
  if (!rotation) {
    lastX = x;
    lastY = y;
    translation = true;
  }
}

void ControllerTurntable::translateEnd(int x, int y) {
  if (translation) {
    deltaX = x - lastX;
    deltaY = y - lastY;
    translation = false;
  }
}

void ControllerTurntable::rotateStart(int x, int y) {
  if (!translation) {
    lastX = x;
    lastY = y;
    rotation = true;
  }
}

void ControllerTurntable::rotateEnd(int x, int y) {
  if (rotation) {
    deltaX = x - lastX;
    deltaY = y - lastY;
    rotation = false;
  }
}

void ControllerTurntable::motion(int x, int y) {
  if (translation) {
    deltaX = x - lastX;
    deltaY = y - lastY;
  } else if (rotation) {
    deltaX = x - lastX;
    deltaY = y - lastY;
  }
}

void ControllerTurntable::apply(Camera &camera) {
  if (translation) {
    camera.translatePixels(deltaX, deltaY);
  }

  else if (rotation) {
    camera.rotatePixels(-deltaX, -deltaY);
  }

  lastX += deltaX;
  lastY += deltaY;
  deltaX = 0;
  deltaY = 0;

  if (zoomFactor != 0) {
    camera.zoom(zoomFactor);
    zoomFactor = 0;
  }
}

void ControllerTurntable::zoom(bool direction) {
  if (!translation && !rotation) {
    float zoomDelta = .1;

    zoomFactor += zoomDelta * (direction ? 1 : -1);
  }
}

glm::vec3 ControllerTurntable::screenToCameraPlane(const Camera &camera, int x, int y) {
  unsigned int width;
  unsigned int height;
  camera.getSize(width, height);

  glm::vec4 centerProj = camera.projection * camera.view * glm::vec4(camera.center, 1.0);
  centerProj /= centerProj.w;

  glm::vec4 screenPos(x, y, centerProj.z, 1.f);

  glm::mat4 screenToClipSpace(1.f);
  screenToClipSpace = glm::scale(screenToClipSpace, {1.f, -1.f, 1.f});
  screenToClipSpace = glm::translate(screenToClipSpace, {-1.f, -1.f, 0.f});
  screenToClipSpace = glm::scale(screenToClipSpace, {2.f / width, 2.f / height, 1.f});

  glm::mat4 clipToViewSpace(1.f);
  clipToViewSpace = glm::inverse(camera.projection * camera.view);

  glm::vec4 viewPos = clipToViewSpace * screenToClipSpace * screenPos;
  viewPos /= viewPos.w;

  return viewPos;
}
