#include "Hitbox.hpp"


Hitbox::Hitbox(glm::vec3 position)
	:size({ 0.6f, 1.8f, 0.6f }), pos(position),
	speed({ 0.0f, 0.0f, 0.0f }), acceleration({ 0.0f, 0.0f, 0.0f }),character({0.0f,0.0f,0.0f},{0.0f,0.0f})
	, mode(Mode::WALKING),view(View::FIRST_PERSON)
{

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

void Hitbox::move(glm::vec3 amount) {
	glm::vec3 rotation = character.getHeadProperties().reachRotation + character.getBodyRotation();
	pos.z += -cos(glm::radians(rotation.y))*amount.z;
	pos.x += -sin(glm::radians(rotation.y)) * amount.z;

	pos.x += -cos(glm::radians(rotation.y)) * amount.x;
	pos.z += sin(glm::radians(rotation.y)) * amount.x;

	if (character.getHeadProperties().reachRotation.y != 0) {
		float rotate = 5.0 * amount.z * (character.getHeadProperties().reachRotation.y > 0 ? -1 : 1);
		if (fabs(rotate) > fabs(character.getHeadProperties().reachRotation.y)) rotate = character.getHeadProperties().reachRotation.y;
		character.rotateBody({ 0.0f,rotate,0.0f });
		character.rotateHead({ 0.0f,-rotate });
	}

	pos.y += amount.y;

}


void Hitbox::drawCharacter() {
	character.draw(glm::translate(glm::mat4(1.f), pos), view ==View::FIRST_PERSON);

}
