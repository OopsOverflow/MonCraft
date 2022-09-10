#pragma once

#include <glm/glm.hpp>

#include "entity/character/Character.hpp"
#include "ui/Key.hpp"

namespace Config { struct ClientConfig; }

class KeyboardController {
public:
	KeyboardController();

  bool handleKeyPressed(Key k);
  bool handleKeyReleased(Key k);
  void apply(Character& character);

private:
  bool isDoubleSpace();

  Config::ClientConfig& config;

  glm::vec3 direction;
  CharacterView view;

  bool sprint;
  bool toggleGod;
  bool dab;

  bool spaceIsPressed;
  uint32_t lastSpacePress;
};
