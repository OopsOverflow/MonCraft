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
	Entity(Hitbox hitbox);

	virtual ~Entity();

	/**
	 * Sets a target moving direction. vec3(0) means stopping.
	 */
	void walk(glm::vec3 direction);

	/**
	 * Turns the entity's view direction by a specified amount in global x/y axes.
	 */
	void turn(glm::vec2 rotation);

	/**
 	 * Moves a camera to the head of this creature. The camera will be the eyes
	 * of this entity.
	 */
	void cameraToHead(Camera& cam);

	/**
	 * Renders the entity.
	 */
	virtual void render() = 0;

	/**
	 * Update the state of the entity
	 */
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
	float playerFovY;

	glm::vec3 speed;
	glm::vec3 accel;
	glm::vec3 direction;

	bool onFloor;
	Hitbox hitbox;

	Node node;
	Node headNode;
};
