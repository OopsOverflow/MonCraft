#pragma once

#include <glm/glm.hpp>

#include "entity/Hitbox.hpp"
#include "entity/Node.hpp"
#include "util/Raycast.hpp"
#include "multiplayer/Serialize.hpp"
#include "terrain/BlockArray.hpp"

enum class State { Walking, Idle };

enum class PlayerView { FIRST_PERSON, THIRD_PERSON, FRONT };

//We will compare the actions with the normal character
enum class Action{ACTION_0_DOWN, ACTION_0_UP, //left mouse click
ACTION_1_DOWN, ACTION_1_UP, //middle mouse click
ACTION_2_DOWN, ACTION_2_UP, //right mouse click
ACTION_3_DOWN, ACTION_3_UP, //sprint button
ACTION_4_DOWN, ACTION_4_UP,	//dab button
ACTION_5_DOWN, ACTION_5_UP,
};

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
	 * Gets the entity terrain modification since the last call to this method.
	 */
  	BlockArray& getRecord();

	/**
	 * Update the state of the entity
	 */
	virtual void update(uint32_t dt);

	virtual void render();

	glm::vec3 getPosition() const;

	void setPosition(glm::vec3 pos);

	virtual void handleAction(Action action) = 0;

	virtual void updateProperties();

	virtual void consume(sf::Packet& packet);
	virtual sf::Packet& operator<<(sf::Packet& packet);
	virtual sf::Packet& operator>>(sf::Packet& packet) const;

	State state;

	Node bodyNode;
	Node headNode;

	EntityProperties properties;

	glm::vec3 speed;
	glm::vec3 accel;
	glm::vec3 direction;

	PlayerView view;

	bool god;
	bool onFloor;

	bool hasBreak;
	bool breaked;

	Raycast caster;

	Hitbox hitbox;

protected: 
	BlockArray record;
};
