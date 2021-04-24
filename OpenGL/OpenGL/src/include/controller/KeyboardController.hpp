#pragma once

#include "../entity/Hitbox.hpp"
#include "../include/Music.hpp"
#include <chrono>

class KeyboardController {
public:
	KeyboardController();

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
  void pressedF5();
  void pressedN();

  void apply(Hitbox& character);

private:
	Music player;
  float speed;
  glm::vec3 direction;
  View view;
  std::chrono::time_point<std::chrono::steady_clock> timer;
};
