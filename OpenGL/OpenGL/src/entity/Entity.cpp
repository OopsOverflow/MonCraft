#include "Entity.hpp"

using namespace glm;
static const mat4 I(1.f);

Entity::Entity(Hitbox hitbox)
: maxSpeed(4.3f), maxAccel(10.f), verticalFriction(5.5f), horizontalFriction(5.f),
	gravity(32.f), jumpSpeed(10.5f), maxFallSpeed(78.4f),
	speed(0), accel(0),
	onFloor(false), god(true), sprint(false),
	state(State::Idle),
	hitbox(std::move(hitbox)),
	playerFovY(45.0f)
{}

Entity::~Entity() {}

void Entity::walk(vec3 dir) {

	if(god) {
		if(dir == vec3(0)) {
			state = State::Idle;
			sprint = false;
			accel = vec3(0);
		}
		else {
			state = State::Walking;
			direction = normalize(dir);
			accel = direction * maxAccel * 4.0f;
		}
	}

	else { // not god
		float dirY = dir.y;
		dir.y = 0;
		if(dir == vec3(0)) {
			state = State::Idle;
			sprint = false;
			accel = vec3(0);
		}
		else {
			state = State::Walking;
			direction = normalize(dir);
			accel = direction * maxAccel;
		}
		if(dirY > 0 && onFloor) {
			onFloor = false;
			speed.y = jumpSpeed;
		}
	}
}

void Entity::turn(vec2 rot) {
	headNode.rot.x += rot.x;
	float maxRotX = radians(89.f);
	headNode.rot.x = clamp(headNode.rot.x, -maxRotX, maxRotX);

	float thresold = quarter_pi<float>();
	float headDelta = clamp(headNode.rot.y + rot.y, -thresold, thresold) - headNode.rot.y;

	headNode.rot.y += headDelta;
	node.rot.y += rot.y - headDelta;
}

void Entity::cameraToHead(Camera& camera) {
	if(view == View::FIRST_PERSON) {
		vec3 eyePos = headNode.model * vec4(0, 4, 4, 1);
		vec3 eyeTarget = headNode.model * vec4(0, 4, 54, 1);
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

vec3 normalizeOrZero(vec3 vec) {
	return all(lessThan(abs(vec), vec3(0.0001f))) ? vec3(0) : normalize(vec);
}

void Entity::update(Terrain& terrain, float dt) {
	// update forces
	vec3 posOffset;
	{
		vec3 acc = accel;
		if(!god) acc += vec3(0, -1, 0) * gravity; // gravity

		// disable friction in accel direction
		vec2 dragXZ = vec2(speed.x, speed.z) * horizontalFriction;
		float dragY = 0;
		if (god)dragY = speed.y * verticalFriction;
		vec2 accXZ = vec2(acc.x, acc.z);
		if(accXZ != vec2(0))
			dragXZ -= normalize(accXZ) * max(dot(dragXZ, normalize(accXZ)), 0.f); // substract component in accel direction from drag

		// update speed
		speed = speed + acc * dt - vec3(dragXZ.x, dragY, dragXZ.y) * dt;
		vec2 speedXZ = vec2(speed.x, speed.z);
		if (god) {
			maxSpeed = 10.89f * 3.0f;
			if (sprint) maxSpeed = 10.89f * 2.0f;
		}
		else {
			maxSpeed = 4.317f * 3.0f;
			if (sprint) maxSpeed = 4.317f * 1.3f;
		}
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
		vec3 newPos = node.loc;
		float totalOffset = length(posOffset);
		size_t steps = (size_t)ceil(totalOffset);
		for (size_t i = 0; i < steps; i++) {
			vec3 thisOffset = posOffset * 1.f / (float)steps;
			newPos = hitbox.computeCollision(newPos, thisOffset, terrain);
		}

		// the final entity displacement for this frame.
		vec3 finalOffset = newPos - (node.loc + posOffset);
		node.loc = newPos;

		// on ground
		if(finalOffset.y > 0.0001) {
			onFloor = true;
			god = false;
		}

		// cancel speed in collision direction
		if(length(finalOffset) > 0.0001) {
			auto rotMatrix = rotate(I, -node.rot.y - headNode.rot.y, {0, 1, 0});
			vec3 worldOffDir = normalize(vec3(rotMatrix * vec4(finalOffset, 1.f)));
			if(finalOffset != vec3(0)) speed -= worldOffDir * dot(speed, worldOffDir); // substract component in collision direction from speed
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
