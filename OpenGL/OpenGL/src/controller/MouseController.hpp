#pragma once

#include "../camera/Camera.hpp"
#include "../entity/Hitbox.hpp"

class MouseController {
public:
	MouseController();

  void rotateStart(int x, int y);
  void rotateEnd(int x, int y);
  void motion(int x, int y);


  void apply(Hitbox& character, Camera& camera);

private:
  int lastX;
  int lastY;
  int deltaX;
  int deltaY;
  bool rotation;

  float speed;
};
