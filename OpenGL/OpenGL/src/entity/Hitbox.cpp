#include "Hitbox.hpp"

using namespace std::chrono;

Hitbox::Hitbox(glm::vec3 position)
	:size({ 0.6f, 1.8f, 0.6f }), pos(position),
	speed(0.0f), character({0.0f,0.0f,0.0f},{0.0f,0.0f})
	, mode(Mode::WALKING),view(View::FIRST_PERSON)
{
	timer = steady_clock::now();
}

void Hitbox::cameraToHead(Camera& camera) {
	glm::vec3 cameraRot = character.getBodyRotation() + character.getHeadProperties().reachRotation;
	camera.setRotation(cameraRot);
	glm::vec3 cameraPos = pos + character.getHeadProperties().localPosition;
	if (view == View::THIRD_PERSON) {
		glm::vec4 newPos(0.0f, 0.0f, -8.5f, 1.0f);
		newPos = glm::rotate(glm::mat4(1.0f), glm::radians(-cameraRot.x), { 1.0f,0.0f,0.0f }) * newPos;
		newPos = glm::rotate(glm::mat4(1.0f), glm::radians(cameraRot.y), { 0.0f,1.0f,0.0f }) * newPos;
		cameraPos += glm::vec3(newPos);

	}
	camera.setPosition(cameraPos);
}

void Hitbox::move(glm::vec3 direction) {
	glm::vec3 rotation = character.getHeadProperties().reachRotation + character.getBodyRotation();
	pos.z += -cos(glm::radians(rotation.y))* direction.z;
	pos.x += -sin(glm::radians(rotation.y)) * direction.z;

	pos.x += -cos(glm::radians(rotation.y)) * direction.x;
	pos.z += sin(glm::radians(rotation.y)) * direction.x;
	if (direction.x != 0 || direction.z!=0) {
		float rotate = 45 * direction.x * (direction.z > 0 ? -1 : 1);
		rotate = character.getHeadProperties().reachRotation.y-rotate;
		if (fabs(rotate) > 5.0f) {
			rotate = 5.0f * (rotate > 0 ? 1 : -1);
		}

		character.rotateBody({ 0.0f,rotate,0.0f });
		character.rotateHead({ 0.0f,-rotate });

		character.setMemberRotation({ 45.0f * speed / 5.0f,0.0f }, Member::LEFT_LEG);
		character.setMemberRotation({ -45.0f * speed / 5.0f,0.0f }, Member::RIGHT_LEG);
	}
	else {
		character.setMemberRotation({ 0.0f,0.0f }, Member::LEFT_LEG);
		character.setMemberRotation({ 0.0f,0.0f }, Member::RIGHT_LEG);

	}



	pos.y += direction.y;

}

void Hitbox::animate() {
	auto cur_timer = steady_clock::now();
	auto elapsed = duration_cast<milliseconds>(cur_timer - timer).count();
	timer = cur_timer;

	character.bodyReachRotation(1000.0 / elapsed * 0.25f);
	character.headReachRotation(1000.0 / elapsed * 0.25f);
	character.memberReachRotation(1000.0 / elapsed * 0.25f, Member::LEFT_ARM);
	character.memberReachRotation(1000.0 / elapsed * 0.25f, Member::RIGHT_ARM);
	character.memberReachRotation(1000.0 / elapsed * 0.15f, Member::LEFT_LEG);
	character.memberReachRotation(1000.0 / elapsed * 0.15f, Member::RIGHT_LEG);
}


void Hitbox::drawCharacter() {
	character.draw(glm::translate(glm::mat4(1.f), pos), view ==View::FIRST_PERSON);

}
