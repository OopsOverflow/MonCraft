#pragma once

#include "../gl/Camera.hpp"
#include "../entity/Entity.hpp"

class MouseController {
public:
	MouseController();

  void rotateStart(int x, int y);
  void rotateEnd(int x, int y);
  void motion(int x, int y);


  void apply(Entity& character, Camera& camera);

private:
  int lastX;
  int lastY;
  int deltaX;
  int deltaY;
  bool rotation;

  float speed;
};
