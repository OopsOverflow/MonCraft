#pragma once

#include "../entity/Entity.hpp"

class MouseController {
public:
	MouseController();

  void rotateStart(int x, int y);
  void rotateEnd(int x, int y);
  void motion(int x, int y);
  void motionRel(int dx, int dy);


  void apply(Entity& character);

private:
  int lastX;
  int lastY;
  int deltaX;
  int deltaY;
  bool rotation;

	float sensivity; // rotation radians per mouse pixel on screen
};
