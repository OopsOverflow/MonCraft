#include "Entity.hpp"

using namespace glm;
static const highp_dmat4 I(1.0);

Entity::Entity(Hitbox hitbox)
: view(View::FIRST_PERSON), state(State::Idle),
	maxSpeed(4.3f), maxAccel(10.f), verticalFriction(0.f), horizontalFriction(5.f),
	gravity(32.f), jumpSpeed(10.5f), maxFallSpeed(78.4f),
	playerFovY(45.0f),
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
		accel = direction * maxAccel;
	}
}

void Entity::jump() {
	if(onFloor) {
		onFloor = false;
		speed.y = jumpSpeed;
	}
}

void Entity::turn(vec2 rot) {
	headNode.rot.x += rot.x;
	auto maxRotX = radians(89.0);
	headNode.rot.x = clamp(headNode.rot.x, -maxRotX, maxRotX);

	auto thresold = quarter_pi<double>();
	auto headDelta = clamp(headNode.rot.y + rot.y, -thresold, thresold) - headNode.rot.y;

	headNode.rot.y += headDelta;
	node.rot.y += rot.y - headDelta;
}

void Entity::cameraToHead(Camera& camera) {
	if(view == View::FIRST_PERSON) {
		vec3 eyePos = headNode.model * vec4(0, 4, 0, 1);
		vec3 eyeTarget = headNode.model * vec4(0, 4, 50, 1);
		camera.setLookAt(eyePos, eyeTarget);
	}
	else {
		vec3 eyePos = headNode.model * vec4(0, 4, 4, 1);
		vec3 eyeTarget = headNode.model * vec4(0, 4, -100, 1);
		camera.setLookAt(eyeTarget, eyePos);
	}
	camera.setFovY(playerFovY);
}

#include "../debug/Debug.hpp"

void Entity::update(Terrain& terrain, float dt) {
	// update forces
	// highp_dvec3 posOffset;
	highp_dvec3 posOffset;
	{
		vec3 acc = accel;
		acc += vec3(0, -1, 0) * gravity; // gravity

		// disable friction in accel direction
		vec2 dragXZ = vec2(speed.x, speed.z) * horizontalFriction;
		float dragY = 0;
		dragY = speed.y * verticalFriction;
		vec2 accXZ = vec2(acc.x, acc.z);
		if(accXZ != vec2(0))
			dragXZ -= normalize(accXZ) * max(dot(dragXZ, normalize(accXZ)), 0.f); // substract component in accel direction from drag

		// update speed
		speed = speed + acc * dt - vec3(dragXZ.x, dragY, dragXZ.y) * dt;
		vec2 speedXZ = vec2(speed.x, speed.z);
		if(length(speedXZ) >= maxSpeed) {
			speedXZ = normalize(speedXZ) * maxSpeed;
			speed = vec3(speedXZ.x, speed.y, speedXZ.y);
		}
		speed.y = max(speed.y, -maxFallSpeed);

		// apply motion
		auto rotMatrix = rotate(I, node.rot.y + headNode.rot.y, {0, 1, 0});
		posOffset = vec3(rotMatrix * vec4(speed * dt, 1.f));
	}

	// check collisions
	{
		// check collisions one block at a time (usually posOffset is < 1 so only 1 check)
		auto newPos = node.loc;
		float totalOffset = length(posOffset);
		size_t steps = (size_t)ceil(totalOffset);
		for (size_t i = 0; i < steps; i++) {
			vec3 thisOffset = posOffset * 1.0 / (double)steps;
			newPos = hitbox.computeCollision(newPos, thisOffset, terrain);
		}

		// the final entity displacement for this frame.
		highp_dvec3 finalOffset = newPos - (node.loc + posOffset);
		node.loc = newPos;

		// on ground
		if(finalOffset.y > 0.001) {
			onFloor = true;
		}

		// cancel speed in collision direction
		if(length(finalOffset) > 0.001) {
			auto rotMatrix = rotate(I, -node.rot.y - headNode.rot.y, {0, 1, 0});
			vec3 worldOffDir = normalize(rotMatrix * highp_dvec4(finalOffset, 1.0));
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

		float currentRot = -headNode.rot.y;
		float dist = targetRot - currentRot;

		float speed = 4.f;
	  float delta = min(speed * dt, abs(dist)) * sign(dist);

	  node.rot.y += delta;
	  headNode.rot.y -= delta;
	}

	playerFovY = playerFovY - (playerFovY - (45.0f + length(speed) / 4.0f)) * 10.0f * dt;

	node.update();
}

vec3 Entity::getPosition() const {
	return node.loc;
}
