#include "controller/KeyboardController.hpp"

using namespace std::chrono;

KeyboardController::KeyboardController() {
  speed = 1.f;
  timer = steady_clock::now();
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

glm::vec3 normalizeOrZero(glm::vec3 vec) {
  if (vec.x == 0 && vec.y == 0 && vec.z == 0)
    return vec;
  return glm::normalize(vec);
}

void KeyboardController::apply(Hitbox& character) {
  auto cur_timer = steady_clock::now();
  auto elapsed = duration_cast<milliseconds>(cur_timer - timer).count();
  timer = cur_timer;

  if (direction.x == 0 && direction.y == 0 && direction.z == 0)
    return;

  float translate_amount = speed * elapsed * 1e-3f;

  character.move(normalizeOrZero(glm::vec3(translate_amount,1.0f,1.0f)));
}
