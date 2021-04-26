#include "Member.hpp"

Member::Member(Mesh mesh)
  : mesh(std::move(mesh))
{}

Member::~Member() {
  
}

void Member::rotate(glm::vec2 rotation) {

}

void Member::updateRotation(float maxRotation) {
    glm::vec2 rot = targetRot - currentRot;
    float totalRot = glm::degrees(acos(cos(rot.x) * cos(rot.y)));//merci Timothée Braud :)
    if (totalRot > maxRotation) {
        currentRot += rot * maxRotation / totalRot;
    }
    else {
        currentRot = targetRot;
    }

    glm::vec3 pivotLocation;

    if (member <= 2) {
        pivotLocation = glm::vec3(0.0f, (2 * 12 / 6), 0.0f);
    }
    else {
        pivotLocation = glm::vec3(0.0f, (3 * 12 / 6), 0.0f);
    }
    //std::cout << model->localRotation.x <<", "<<model->localRotation.y<<std::endl;

    model->localMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(4, 12, 4));
    model->localMatrix = glm::translate(glm::mat4(1.0f), -pivotLocation) * model->localMatrix;
    model->localMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(model->localRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * model->localMatrix;
    model->localMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(model->localRotation.x), glm::vec3(cos(glm::radians(model->localRotation.y)), 0.0f, -sin(glm::radians(model->localRotation.y)))) * model->localMatrix;

    model->localMatrix = glm::translate(glm::mat4(1.0f), pivotLocation) * model->localMatrix;
}

void Member::draw() {

}

void Member::addChild(std::unique_ptr<Member> child) {
  children.push_back(std::move(child));
}
