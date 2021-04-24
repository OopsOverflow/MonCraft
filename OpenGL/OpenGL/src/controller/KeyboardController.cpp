#include "controller/KeyboardController.hpp"

using namespace std::chrono;

KeyboardController::KeyboardController() {
  view = View::THIRD_PERSON;
  speed = 3.f;
  direction = glm::vec3(0.f);
}

void KeyboardController::pressedForward() {
  direction.z = -1;
}

void KeyboardController::releasedForward() {
  if (direction.z == -1)
    direction.z = 0;
}

void KeyboardController::pressedBackward() {
  direction.z = 1;
}

void KeyboardController::releasedBackward() {
  if (direction.z == 1)
    direction.z = 0;
}

void KeyboardController::pressedLeft() {
  direction.x = -1;
}

void KeyboardController::releasedLeft() {
  if (direction.x == -1)
    direction.x = 0;
}

void KeyboardController::pressedRight() {
  direction.x = 1;
}

void KeyboardController::releasedRight() {
  if (direction.x == 1)
    direction.x = 0;
}

void KeyboardController::pressedUp() {
  direction.y = 1;
}

void KeyboardController::releasedUp() {
  if (direction.y == 1)
    direction.y = 0;
}

void KeyboardController::pressedDown() {
  direction.y = -1;
}

void KeyboardController::releasedDown() {
  if (direction.y == -1)
    direction.y = 0;
}

void KeyboardController::pressedF5() {
    if (view == View::FIRST_PERSON) {
        view = View::THIRD_PERSON;
    }
    else {
        view = View::FIRST_PERSON;
    }
}


//glm::vec3 normalizeOrZero(glm::vec3 vec) {
//  if (vec.x == 0 && vec.y == 0 && vec.z == 0)
//    return vec;
//  return glm::normalize(vec);
//}

void KeyboardController::apply(Hitbox& character) {

  character.view = view;
  character.move(glm::vec3(direction.x ,direction.y, direction.z));
}
