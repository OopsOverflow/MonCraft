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
	void setSprint();
	void walk(glm::vec3 direction);
	void turn(glm::vec2 rotation);
	void cameraToHead(Camera& cam);
	virtual void render() = 0;
	virtual void update(Terrain& terrain, float dt);
	View view;
	State state;
	bool god;
	bool sprint;

protected:

	float maxSpeed;
	float maxAccel;
	float verticalFriction;
	float horizontalFriction;
	float gravity;
	float jumpSpeed;
	float maxFallSpeed;

	glm::vec3 speed;
	glm::vec3 accel;

	bool onFloor;


	Hitbox hitbox;
	glm::vec3 direction;

	Node node;
	Node headNode;
};
