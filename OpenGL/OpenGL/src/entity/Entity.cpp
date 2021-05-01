#include "Entity.hpp"

using namespace glm;
static const mat4 I(1.f);

Entity::Entity()
: maxSpeed(10), maxAccel(1), friction(5),
	speed(0), accel(0),
	state(State::Idle)
{ }

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

void Entity::turn(vec2 rot) {
	headNode.rot.x += rot.x;
	float maxRotX = radians(85.f);
	headNode.rot.x = clamp(headNode.rot.x, -maxRotX, maxRotX);

	float thresold = quarter_pi<float>();
	float headDelta = clamp(headNode.rot.y + rot.y, -thresold, thresold) - headNode.rot.y;

	headNode.rot.y += headDelta;
	node.rot.y += rot.y - headDelta;
}

void Entity::cameraToHead(Camera& camera) {
	if(view == View::FIRST_PERSON) {
		vec3 eyePos = headNode.model * vec4(0, 4, 4, 1);
		vec3 eyeTarget = headNode.model * vec4(0, 4, 5, 1);
		camera.setLookAt(eyePos, eyeTarget);
	}
	else {
		vec3 eyePos = headNode.model * vec4(0, 4, 4, 1);
		vec3 eyeTarget = headNode.model * vec4(0, 4, -100, 1);
		camera.setLookAt(eyeTarget, eyePos);
	}
}

#include "../debug/Debug.hpp"

void Entity::update(float dt) {
	// update forces
	{
		// disable friction in accel direction
		vec3 drag = speed * friction;
		if(accel != vec3(0))
			drag -= normalize(accel) * max(dot(drag, normalize(accel)), 0.f); // substract component in accel direction from drag

		// update speed
		speed = speed + accel * dt - drag * dt;
		if(length(speed) >= maxSpeed)
			speed = normalize(speed) * maxSpeed;

		// apply motion
		auto rotMatrix = rotate(I, node.rot.y + headNode.rot.y, {0, 1, 0});
		node.loc += vec3(rotMatrix * vec4(speed, 1.f));
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

	node.update();
}
