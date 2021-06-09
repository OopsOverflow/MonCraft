#pragma once

#include "entity/character/Character.hpp"
#include "terrain/Terrain.hpp"

#include <glm/glm.hpp>

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
  void pressedControl();
  void releasedControl();
  void pressedPause();

  void pressedF5();
  void changedMod();

  void apply(Character& character, Terrain& terrain);

private:
  glm::vec3 direction;
  State state;
  View view;

  bool sprint;
  bool change;
  bool paused;
};
