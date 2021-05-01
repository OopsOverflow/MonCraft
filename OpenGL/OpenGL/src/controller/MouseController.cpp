#include "MouseController.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

MouseController::MouseController() {
  lastX = lastY = deltaX = deltaY = 0;
  sensivity = 0.005;
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


void MouseController::apply(Entity& character) {

  if (rotation) {
    character.turn(glm::vec2(deltaY, -deltaX) * sensivity);
  }

  lastX += deltaX;
  lastY += deltaY;
  deltaX = 0;
  deltaY = 0;

}
