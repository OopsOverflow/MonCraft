#include "MouseController.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

MouseController::MouseController() {
  lastX = lastY = deltaX = deltaY = 0;
  sensivity = 0.005f;
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

void MouseController::triggerAction(MouseController::Action action) {
  actions.push_back(action);
}

void MouseController::apply(Character& character, Terrain& terrain) {

  if (rotation) {
    character.turn(glm::vec2(deltaY, -deltaX) * sensivity);
  }

  lastX += deltaX;
  lastY += deltaY;
  deltaX = 0;
  deltaY = 0;

  for(auto action : actions) switch (action) {
    case Action::PLACE:
      character.placeBlock(terrain);
      break;
    case Action::DESTROY:
      character.breakBlock(terrain);
      break;
    case Action::PICK:
      character.pickBlock(terrain);
      break;
    default:
      std::cout << "MouseController: Action not supported." << std::endl;
      break;
  };
  actions = {};
}
