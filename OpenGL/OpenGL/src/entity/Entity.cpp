#include "Entity.hpp"


Entity::Entity() {

}


Entity::~Entity() {

}


void Entity::move(glm::vec3 direction) {
	glm::vec3 rotation = character.getHeadProperties().reachRotation + character.getBodyRotation();
	pos.z += -cos(glm::radians(rotation.y)) * direction.z;
	pos.x += -sin(glm::radians(rotation.y)) * direction.z;

	pos.x += -cos(glm::radians(rotation.y)) * direction.x;
	pos.z += sin(glm::radians(rotation.y)) * direction.x;
	if (direction.x != 0 || direction.z != 0) {
		float rotate = 45 * direction.x * (direction.z > 0 ? -1 : 1);
		rotate = character.getHeadProperties().reachRotation.y - rotate;
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
}

void Entity::rotate(glm::vec2 rotation) {

}
