#include "Entity.hpp"

using namespace glm;
static const mat4 I(1.f);

Entity::Entity()
: accel(0), maxAccel(1), speed(0), maxSpeed(10), friction(2),
	state(State::Idle)
{

}

Entity::~Entity() {}


void Entity::walk(glm::vec3 dir) {
	if(dir == vec3(0)) {
		state = State::Idle;
		accel = min(accel, 0.f);
	}
	else {
		dir = normalize(dir);
		state = State::Walking;
		speed *= max(dot(direction, dir), 0.f);
		direction = dir;
		if(speed >= maxSpeed) {
			speed = maxSpeed;
			accel = 0;
		}
		accel = maxAccel;
	}
}

void Entity::turn(glm::vec2 rot) {
	headNode.rot.x += rot.x;
	// node.rot.y += rot.y;

	float thresold = glm::quarter_pi<float>();
	vec2 headRot = headNode.rot;
	float headDelta = clamp(headRot.y + rot.y, -thresold, thresold) - headRot.y;

	headNode.rot.y += headDelta;
	node.rot.y += rot.y - headDelta;
}

void Entity::cameraToHead(Camera& camera) {
	if(view == View::FIRST_PERSON) {
		vec3 eyePos = headNode.model * glm::vec4(0, 4, 4, 1);
		vec3 eyeTarget = headNode.model * glm::vec4(0, 4, 5, 1);
		camera.setLookAt(eyePos, eyeTarget);
	}
	else {
		vec3 eyePos = headNode.model * glm::vec4(0, 4, 4, 1);
		vec3 eyeTarget = headNode.model * glm::vec4(0, 4, -100, 1);
		camera.setLookAt(eyeTarget, eyePos);
	}
}

#include "../debug/Debug.hpp"

void Entity::update(float dt) {

	// update forces
	{
		float oldSpeed = speed;
		float oldAccel = accel;

		speed = max(oldSpeed + oldAccel * dt, 0.f);
		accel -= friction * oldSpeed * dt;
		auto rotMatrix = glm::rotate(I, node.rot.y + headNode.rot.y, {0, 1, 0});
		node.loc += vec3(rotMatrix * vec4(direction, 1.f)) * speed;
	}

	// update chest rotation based on direction
	if(state == State::Walking || true) {

		float targetRot = 0; // offset body rotation when strafing (not walking straight ahead)
		float thresold = glm::quarter_pi<float>();
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
