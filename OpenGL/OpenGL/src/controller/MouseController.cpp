#include "MouseController.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

MouseController::MouseController() {
  lastX = lastY = deltaX = deltaY = 0;
  speed = 0.f;
  rotation = false;
}


void MouseController::rotateStart(int x, int y) {
    lastX = x;
    lastY = y;
    rotation = true;
}

void MouseController::rotateEnd(int x, int y) {
  if (rotation) {
    deltaX = x - lastX;
    deltaY = y - lastY;
    rotation = false;
  }
}

void MouseController::motion(int x, int y) {
    if (rotation) {
    deltaX = x - lastX;
    deltaY = y - lastY;
  }
}

void MouseController::motionRel(int dx, int dy) {
    if (rotation) {
    deltaX += dx;
    deltaY += dy;
  }
}

void MouseController::apply(Entity& character, Camera& camera) {

  unsigned int screenWidth;
  unsigned int screenHeight;

  float maxRotation = glm::pi<float>();
  camera.getSize(screenWidth, screenHeight);
  if (rotation) {
    character.turn(glm::vec2(deltaY / (float)screenHeight, -deltaX / (float)screenWidth) * maxRotation);
  }

  lastX += deltaX;
  lastY += deltaY;
  deltaX = 0;
  deltaY = 0;

}
