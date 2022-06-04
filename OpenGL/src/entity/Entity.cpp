#include "Entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <cmath>
#include <utility>

#include "entity/Hitbox.hpp"
#include "entity/Node.hpp"

using namespace glm;
static const highp_dmat4 I(1.0);

float Entity::gravity = 32.f;

Entity::Entity(Hitbox hitbox, EntityProperties properties) :
	state(State::Idle),
	properties(properties),
	speed(0), accel(0), direction(0),
  	onFloor(false),
	hitbox(std::move(hitbox))
{}

Entity::~Entity() {}

void Entity::walk(vec3 dir) {
	if(dir == vec3(0)) {
		state = State::Idle;
		accel = vec3(0);
	}
	else {
		state = State::Walking;
		direction = normalize(dir);
		accel = direction * properties.maxAccel;
	}
}

void Entity::jump() {
	if(onFloor) {
		onFloor = false;
		speed.y = properties.jumpSpeed;
	}
}

void Entity::turn(vec2 rot) {
	headNode.rot.x += rot.x;
	auto maxRotX = radians(89.9);
	headNode.rot.x = clamp(headNode.rot.x, -maxRotX, maxRotX);

	auto thresold = quarter_pi<double>();
	auto headDelta = clamp(headNode.rot.y + rot.y, -thresold, thresold) - headNode.rot.y;

	headNode.rot.y += headDelta;
	bodyNode.rot.y += rot.y - headDelta;
}

void Entity::update(uint32_t dt) {

	// update forces
	highp_dvec3 posOffset;
	{
		vec3 acc = accel;
		acc += vec3(0, -1, 0) * gravity; // gravity

		// disable friction in accel direction
		vec2 dragXZ = vec2(speed.x, speed.z) * properties.horizontalFriction;
		float dragY = speed.y * properties.verticalFriction;
		vec2 accXZ = vec2(acc.x, acc.z);
		if(accXZ != vec2(0))
			dragXZ -= normalize(accXZ) * max(dot(dragXZ, normalize(accXZ)), 0.f); // substract component in accel direction from drag

		// update speed
		speed = speed + acc * (dt * 0.001f) - vec3(dragXZ.x, dragY, dragXZ.y) * (dt * 0.001f);
		vec2 speedXZ = vec2(speed.x, speed.z);
		if(length(speedXZ) >= properties.maxSpeed) {
			speedXZ = normalize(speedXZ) * properties.maxSpeed;
			speed = vec3(speedXZ.x, speed.y, speedXZ.y);
		}
		speed.y = max(speed.y, -properties.maxFallSpeed);

		// apply motion
		auto rotMatrix = rotate(I, bodyNode.rot.y + headNode.rot.y, {0, 1, 0});
		posOffset = vec3(rotMatrix * vec4(speed * (dt * 0.001f), 1.f));
	}

	// check collisions
	{
		// check collisions one block at a time (usually posOffset is < 1 so only 1 check)
		auto newPos = bodyNode.loc;
		float totalOffset = (float)length(posOffset);
		size_t steps = (size_t)ceil(totalOffset);
		for (size_t i = 0; i < steps; i++) {
			vec3 thisOffset = posOffset * 1.0 / (double)steps;
			newPos = hitbox.computeCollision(newPos, thisOffset);
		}

		// the final entity displacement for this frame.
		highp_dvec3 finalOffset = newPos - (bodyNode.loc + posOffset);
		bodyNode.loc = newPos;

		// on ground
		if(finalOffset.y > 0.001) {
			onFloor = true;
		}

		// cancel speed in collision direction
		if(length(finalOffset) > 0.001) {
			auto rotMatrix = rotate(I, -bodyNode.rot.y - headNode.rot.y, {0, 1, 0});
			vec3 worldOffDir = normalize(vec3(rotMatrix * highp_dvec4(finalOffset, 1.0)));
			speed -= worldOffDir * dot(speed, worldOffDir); // substract component in collision direction from speed
		}
	}

	// update chest rotation based on direction
	if(state == State::Walking) {

		float targetRot = 0; // offset body rotation when strafing (not walking straight ahead)
		float thresold = quarter_pi<float>();
		vec2 dir = normalize(vec2(direction.x, direction.z));

		targetRot = -asin(dir.x);
		if(dir.y > 0) targetRot *= -1;
		targetRot = clamp(targetRot, -thresold, thresold);

		float currentRot = (float)-headNode.rot.y;
		float dist = targetRot - currentRot;

		float speed = 4.f;
	  float delta = min(speed * dt * 0.001f, abs(dist)) * sign(dist);

	  bodyNode.rot.y += delta;
	  headNode.rot.y -= delta;
	}

	bodyNode.update();
}

void Entity::render() {}

vec3 Entity::getPosition() const {
	return bodyNode.loc;
}

void Entity::setPosition(glm::vec3 pos) {
	bodyNode.loc = pos;
}
