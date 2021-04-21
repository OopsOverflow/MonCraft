#include "controller/MouseController.hpp"

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

void MouseController::apply(Hitbox& character, Camera& camera) {

    unsigned int screenWidth;
    unsigned int screenHeight;

    float maxRotation = 360.f;
    camera.getSize(screenWidth, screenHeight);
  if (rotation) {
      character.character.rotateHead({ -deltaY * maxRotation/ (float)screenWidth,-deltaX * maxRotation / (float)screenWidth });
  }

  lastX += deltaX;
  lastY += deltaY;
  deltaX = 0;
  deltaY = 0;

}