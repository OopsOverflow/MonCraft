#pragma once

#include <glm/glm.hpp>

#include "Character.hpp"

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
	void move(glm::vec3 amount);
	void drawCharacter();
	Character character;


private:
	
	glm::vec3 size;
	glm::vec3 pos;
	glm::vec3 speed;
	glm::vec3 acceleration;


	Mode mode;
};
