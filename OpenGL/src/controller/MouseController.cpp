#include "MouseController.hpp"

#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>

#include "save/ClientConfig.hpp"

MouseController::MouseController() {
  lastX = lastY = deltaX = deltaY = 0;
  makeRotation = false;
}


void MouseController::rotateStart(int x, int y) {
    lastX = x;
    lastY = y;
    makeRotation = true;
}

void MouseController::rotateEnd(int x, int y) {
  if (makeRotation) {
    deltaX = x - lastX;
    deltaY = y - lastY;
    makeRotation = false;
  }
}

void MouseController::motion(int x, int y) {
    if (makeRotation) {
    deltaX = x - lastX;
    deltaY = y - lastY;
  }
}

void MouseController::motionRel(int dx, int dy) {
    if (makeRotation) {
    deltaX += dx;
    deltaY += dy;
  }
}

void MouseController::triggerAction(Click action) {
  actions.push_back(action);
}

void MouseController::apply(PlayerController& controller) {

  auto character = controller.getEntity();
  if (makeRotation) {
    character->turn(glm::vec2(deltaY, -deltaX) * Config::getClientConfig().sensivity * 0.0001f);
  }

  lastX += deltaX;
  lastY += deltaY;
  deltaX = 0;
  deltaY = 0;

  for(auto action : actions) switch (action) {
    case Click::LEFT:
      character->leftClick();
      break;
    case Click::MIDDLE:
      character->middleClick();
      break;
    case Click::RIGHT:
      character->rightClick();
      break;
    default:
      assert(false);
      break;
  };
  actions = {};
}
