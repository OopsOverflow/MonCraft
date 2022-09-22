#pragma once

#include <vector>
#include "PlayerController.hpp"

	enum class Click {
		LEFT, MIDDLE, RIGHT
	};
class MouseController {
public:
	MouseController();

  void rotateStart(int x, int y);
  void rotateEnd(int x, int y);
  void motion(int x, int y);
  void motionRel(int dx, int dy);

	void triggerAction(Click action);

  void apply(PlayerController& controller);
  bool getRotation() const {return makeRotation;}

private:
  int lastX;
  int lastY;
  int deltaX;
  int deltaY;
  bool makeRotation;

  
	std::vector<Click> actions;
};
