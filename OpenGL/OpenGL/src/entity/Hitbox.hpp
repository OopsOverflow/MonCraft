#pragma once

#include <glm/glm.hpp>

#include "Character.hpp"
#include "Camera.hpp"
#include <chrono>

enum class Mode { SPECTATOR, FLYING, SWIMMING, WALKING };
enum class View { FIRST_PERSON, THIRD_PERSON };

class Hitbox
{
public:
	Hitbox(glm::vec3 position);
	void move(glm::vec3 amount);
	void animate();
	void drawCharacter();
	void cameraToHead(Camera& camera);
	Character character;
	View view;
	glm::vec3 pos;

private:
	
	glm::vec3 size;
	glm::vec3 speed;
	glm::vec3 acceleration;
	std::chrono::time_point<std::chrono::steady_clock> timer;

	Mode mode;
};
