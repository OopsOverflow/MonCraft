#pragma once

#include <glm/glm.hpp>

#include "Hitbox.hpp"
#include "Member.hpp"
#include "../gl/Camera.hpp"
#include "../audio/SoundEffect.hpp"
#include "Chest.hpp"
#include "Head.hpp"


enum class View { FIRST_PERSON, THIRD_PERSON };
enum class State { Walking };

class Entity
{
public:
	Entity();
	virtual ~Entity();

	void move(glm::vec3 direction);
	void rotate(glm::vec2 rotation);
	virtual void rotateHead(glm::vec2 rotation) = 0;
	virtual void cameraToHead(Camera& cam) = 0;
	virtual void render() = 0;
	virtual void playAnimation(float dt) = 0;
	View view;

protected:
	float accel;
	float speed;

	//All entities have one head and one chest
	Head head;
	Chest chest;

	State state;
	Hitbox hitbox;
	glm::vec3 position;
	glm::vec2 rotation;
};
