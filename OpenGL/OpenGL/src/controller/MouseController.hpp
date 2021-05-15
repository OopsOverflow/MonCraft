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

	// for now only two actions a player can achieve with the mouse: placing and destroying blocks.
	enum class Action {
		PLACE, DESTROY, PICK
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
