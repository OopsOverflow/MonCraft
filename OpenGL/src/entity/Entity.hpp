#pragma once

#include <glm/glm.hpp>

#include "entity/Hitbox.hpp"
#include "entity/Node.hpp"

enum class State { Walking, Idle };

struct EntityProperties {
	float maxSpeed = 4.3f;
	float maxAccel = 10.f;
	float verticalFriction = 0.f;
	float horizontalFriction = 5.f;
	float jumpSpeed = 10.5f;
	float maxFallSpeed = 78.4f;

};

/**
 * An entity is a living thing able to move around and subject to physics
 * (speed / acceleration, gravity, collisions...)
 *
 * It is made of at least a head and a body.
 * The Entity's implementation is responsible for linking correctly the
 * headNode to some other node and linking it's body parts to the main node.
 */

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
	virtual void update(float dt);

	virtual void render();

	glm::vec3 getPosition() const;

	void setPosition(glm::vec3 pos);

	static float gravity;

	State state;

	Node bodyNode;
	Node headNode;

	EntityProperties properties;

	glm::vec3 speed;
	glm::vec3 accel;
	glm::vec3 direction;

	bool onFloor;
	Hitbox hitbox;
};
