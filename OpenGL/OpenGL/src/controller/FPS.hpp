#pragma once

#include "../Camera.hpp"

#include <chrono>

class ControllerFPS {
public:
  ControllerFPS();

  void pressedForward();
  void releasedForward();
  void pressedBackward();
  void releasedBackward();
  void pressedLeft();
  void releasedLeft();
  void pressedRight();
  void releasedRight();
  void pressedUp();
  void releasedUp();
  void pressedDown();
  void releasedDown();

  static glm::vec3 screenToCameraPlane(const Camera &camera, int x, int y);

  void apply(Camera &camera);

private:
  float speed;
  glm::vec3 direction;
  std::chrono::time_point<std::chrono::steady_clock> timer;
};
