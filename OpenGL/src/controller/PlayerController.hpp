#pragma once

#include "Controller.hpp"
#include <glm/glm.hpp>
#include "ui/Key.hpp"

class PlayerController : Controller {
public:
	PlayerController();

  bool handleKeyPressed(Key k);
  bool handleKeyReleased(Key k);

  void update();


  Entity* getEntity() {return entity.get();}
private:

  bool isDoubleSpace();
  glm::vec3 direction;
  bool toggleGod;

  bool spaceIsPressed;
  uint32_t lastSpacePress;

};
