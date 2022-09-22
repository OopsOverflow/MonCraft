#pragma once

#include <glm/glm.hpp>

#include "entity/Hitbox.hpp"
#include "entity/Node.hpp"
#include "util/Raycast.hpp"

enum class State { Walking, Idle };

struct EntityProperties {
	float maxSpeed = 4.3f;
	float maxAccel = 10.f;
	float verticalFriction = 0.f;
	float horizontalFriction = 5.f;
	float jumpSpeed = 10.5f;
	float maxFallSpeed = 78.4f;

};

enum class PlayerView { FIRST_PERSON, THIRD_PERSON, FRONT };

/**
 * An entity is a living thing able to move around and subject to physics
 * (speed / acceleration, gravity, collisions...)
 *
 * It is made of at least a head and a body.
 * The Entity's implementation is responsible for linking correctly the
 * headNode to some other node and linking it's body parts to the main node.
 */
static const float gravity = 32.f;

class Entity
{
public:
	/**
	 * Make an entity with the given hitbox.
	 */
	Entity(Hitbox hitbox, EntityProperties properties = EntityProperties());

	virtual ~Entity();

	/**
	 * Sets a target moving direction. vec3(0) means stopping.
	 */
	void walk(glm::vec3 direction);

	void jump();

	/**
	 * Turns the entity's view direction by a specified amount in global x/y axes.
	 */
	void turn(glm::vec2 rotation);

	/**
	 * Update the state of the entity
	 */
	virtual void update(uint32_t dt);

	virtual void render();

	glm::vec3 getPosition() const;

	void setPosition(glm::vec3 pos);

	virtual void leftClick() = 0;
	virtual void middleClick() = 0;
	virtual void rightClick() = 0;

	State state;

	Node bodyNode;
	Node headNode;

	EntityProperties properties;

	glm::vec3 speed;
	glm::vec3 accel;
	glm::vec3 direction;

	PlayerView view;

	bool sprint;
	bool god;
	bool dab;
	bool onFloor;

	bool hasBreak;
	bool breaked;

	Raycast caster;

	Hitbox hitbox;
};
