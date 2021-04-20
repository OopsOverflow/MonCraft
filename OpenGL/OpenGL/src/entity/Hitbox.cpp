#include "Hitbox.hpp"


Hitbox::Hitbox()
	:size({ 0.6f, 1.8f, 0.6f }), pos({ 0.0f, 64.0f, 0.0f }),
	speed({ 0.0f, 0.0f, 0.0f }), acceleration({ 0.0f, 0.0f, 0.0f }),character({0.0f,0.0f,0.0f},{0.0f,0.0f})
	, mode(Mode::WALKING)
{

}

bool Hitbox::setSwimming()
{
	if (mode == Mode::SWIMMING) return 0;

	size.y = 0.6f;
	mode = Mode::SWIMMING;


	return 1;
}

bool Hitbox::setWalking()
{
	if (mode == Mode::WALKING) return 0;

	size.y = 1.8f;
	mode = Mode::WALKING;
	return 1;
}

bool Hitbox::setFlying()
{
	if (mode == Mode::FLYING) return 0;

	size.y = 1.8f;
	mode = Mode::FLYING;

	return 1;
}

bool Hitbox::setSpectator()
{
	if (mode == Mode::SPECTATOR) return 0;

	size.y = 1.8f;
	mode = Mode::SPECTATOR;

	return 1;
}

void Hitbox::move(glm::vec3 amount) {
	pos.x += amount.x;
	pos.y += amount.x;
	pos.z += amount.x;
}


void Hitbox::drawCharacter() {
	character.draw(glm::translate(glm::mat4(1.f), pos));

}
