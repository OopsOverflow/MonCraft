#include "Hitbox.hpp"

Hitbox::Hitbox()
{
	xSize = 0.6f;
	ySize = 1.8f;
	zSize = 0.6f;

	mode = Mode::WALKING;
}

bool Hitbox::setSwimming()
{
	if (mode == Mode::SWIMMING) return 0;
	
	ySize = 0.6f;
	mode = Mode::SWIMMING;


	return 1;
}

bool Hitbox::setWalking()
{
	if (mode == Mode::WALKING) return 0;

	ySize = 1.8f;
	mode = Mode::WALKING;

	return 1;
}

bool Hitbox::setFlying()
{
	if (mode == Mode::FLYING) return 0;

	ySize = 1.8f;
	mode = Mode::FLYING;

	return 1;
}

bool Hitbox::setSpectator()
{
	if (mode == Mode::SPECTATOR) return 0;

	ySize = 1.8f;
	mode = Mode::SPECTATOR;

	return 1;
}