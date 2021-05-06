#pragma once

#include <glm/glm.hpp>

#include "Hitbox.hpp"
#include "Member.hpp"
#include "Node.hpp"
#include "gl/Camera.hpp"
#include "audio/SoundEffect.hpp"


enum class View { FIRST_PERSON, THIRD_PERSON };
enum class State { Walking, Idle };

class Entity
{
public:
	Entity();
	virtual ~Entity();

	void walk(glm::vec3 direction);
	void turn(glm::vec2 rotation);
	void cameraToHead(Camera& cam);
	virtual void render() = 0;
	virtual void update(float dt);
	View view;

protected:

	float maxSpeed;
	float maxAccel;
	float friction;

	glm::vec3 speed;
	glm::vec3 accel;

	State state;
	Hitbox hitbox;
	glm::vec3 direction;

	Node node;
	Node headNode;
};
