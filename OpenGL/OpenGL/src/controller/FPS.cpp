#include "controller/FPS.hpp"

using namespace std::chrono;

ControllerFPS::ControllerFPS() {
  speed = 10.f;
  timer = steady_clock::now();
  direction = glm::vec3(0.f);
}

void ControllerFPS::pressedForward() {
  direction.z = -1;
}

void ControllerFPS::releasedForward() {
  if (direction.z == -1)
    direction.z = 0;
}

void ControllerFPS::pressedBackward() {
  direction.z = 1;
}

void ControllerFPS::releasedBackward() {
  if (direction.z == 1)
    direction.z = 0;
}

void ControllerFPS::pressedLeft() {
  direction.x = -1;
}

void ControllerFPS::releasedLeft() {
  if (direction.x == -1)
    direction.x = 0;
}

void ControllerFPS::pressedRight() {
  direction.x = 1;
}

void ControllerFPS::releasedRight() {
  if (direction.x == 1)
    direction.x = 0;
}

void ControllerFPS::pressedUp() {
  direction.y = 1;
}

void ControllerFPS::releasedUp() {
  if (direction.y == 1)
    direction.y = 0;
}

void ControllerFPS::pressedDown() {
  direction.y = -1;
}

void ControllerFPS::releasedDown() {
  if (direction.y == -1)
    direction.y = 0;
}

glm::vec3 normalizeOrZero(glm::vec3 vec) {
  if (vec.x == 0 && vec.y == 0 && vec.z == 0)
    return vec;
  return glm::normalize(vec);
}

void ControllerFPS::apply(Camera &camera) {
  auto cur_timer = steady_clock::now();
  auto elapsed = duration_cast<milliseconds>(cur_timer - timer).count();
  timer = cur_timer;

  float translate_amount = speed * elapsed * 1e-3f;
  translate_amount *= 1 + glm::length(camera.position - camera.center);

  camera.translate(normalizeOrZero({0, direction.y, 0}) * glm::vec3(translate_amount), false);
  camera.translate(normalizeOrZero({direction.x, 0, direction.z}) * glm::vec3(translate_amount), true);
}
