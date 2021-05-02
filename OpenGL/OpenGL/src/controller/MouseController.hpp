#pragma once

#include "entity/character/Character.hpp"
#include "terrain/Terrain.hpp"

class MouseController {
public:
	MouseController();

  void rotateStart(int x, int y);
  void rotateEnd(int x, int y);
  void motion(int x, int y);
  void motionRel(int dx, int dy);

	enum class Action {
		PLACE, DESTROY, ATTACK
	};

	void triggerAction(Action action);

  void apply(Character& character, Terrain& terrain);

private:
  int lastX;
  int lastY;
  int deltaX;
  int deltaY;
  bool rotation;

	float sensivity; // rotation radians per mouse pixel on screen

	std::vector<Action> actions;
};
