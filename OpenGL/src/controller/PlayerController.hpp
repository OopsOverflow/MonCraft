#pragma once

#include "Controller.hpp"
#include <glm/glm.hpp>
#include "ui/Key.hpp"

class PlayerController : public Controller {
public:
	PlayerController(std::shared_ptr<Entity> entity);

  bool handleKeyPressed(Key k);
  bool handleKeyReleased(Key k);

  void update();

private:

  bool isDoubleSpace();
  glm::vec3 direction;
  bool toggleGod;

  bool spaceIsPressed;
  uint32_t lastSpacePress;

};
