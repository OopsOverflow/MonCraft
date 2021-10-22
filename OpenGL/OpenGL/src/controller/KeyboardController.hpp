#pragma once

#include "entity/character/Character.hpp"
#include "ui/Key.hpp"
#include "util/SaveManager.hpp"

#include <glm/glm.hpp>

class KeyboardController {
public:
	KeyboardController();

  bool handleKeyPressed(Key k);
  bool handleKeyReleased(Key k);
  void apply(Character& character);

private:
  bool isDoubleSpace();

  Config& config;

  glm::vec3 direction;
  State state;
  View view;

  bool sprint;
  bool toggleGod;

  bool spaceIsPressed;
  float lastSpacePress;
};
