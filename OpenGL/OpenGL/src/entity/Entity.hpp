#pragma once

#include <glm/glm.hpp>

#include "Hitbox.hpp"
#include "Member.hpp"
#include "Node.hpp"
#include "gl/Camera.hpp"
#include "terrain/Terrain.hpp"
#include "audio/SoundEffect.hpp"

enum class View { FIRST_PERSON, THIRD_PERSON };
enum class State { Walking, Idle };

class Entity
{
public:
	Entity(Hitbox hitbox);
	virtual ~Entity();

	void walk(glm::vec2 direction);
	void jump();
	void turn(glm::vec2 rotation);
	void cameraToHead(Camera& cam);
	virtual void render() = 0;
	virtual void update(Terrain& terrain, float dt);
	View view;

protected:

	float maxSpeed;
	float maxAccel;
	float friction;

	glm::vec3 speed;
	glm::vec3 accel;

	bool onFloor;

	State state;
	Hitbox hitbox;
	glm::vec3 direction;

	Node node;
	Node headNode;
};
