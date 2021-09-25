#pragma once

#include <glm/glm.hpp>

#include "Hitbox.hpp"
#include "Member.hpp"
#include "Node.hpp"
#include "gl/Camera.hpp"
#include "terrain/Terrain.hpp"
#include "audio/SoundEffect.hpp"
#include "multiplayer/common/Packet.hpp"
#include "multiplayer/common/Identifier.hpp"

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

	void jump();

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
	 * Update the state of the entity
	 */
	virtual void update(Terrain& terrain, float dt);

	glm::vec3 getPosition() const;

	void setPosition(glm::vec3 pos);

	View view;
	State state;

	friend sf::Packet& operator<<(sf::Packet& packet, Entity const& entity);
	friend sf::Packet& operator>>(sf::Packet& packet, Entity& entity);
	sf::Packet& consume(sf::Packet& packet);

	Node getHead() const { return headNode; }
	Node getNode() const { return node; }

	void setHead(Node node) { this->headNode = node; }
	void setNode(Node node) { this->node = node; }

	Identifier getIdentifier() const;

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

private:
	Identifier uid;
};
