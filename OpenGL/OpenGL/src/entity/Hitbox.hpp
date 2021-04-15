#pragma once

#include <glm/glm.hpp>

enum class Mode { SPECTATOR, FLYING, SWIMMING, WALKING };
enum class View { FIRST_PERSON, THIRD_PERSON };

class Hitbox
{
public:
	Hitbox();
	bool setSwimming();
	bool setWalking();
	bool setFlying();
	bool setSpectator();

private:
	float xSize;
	float ySize;
	float zSize;

	Mode mode;
};


