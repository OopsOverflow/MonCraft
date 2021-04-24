#include "Member.hpp"

Member::Member(Mesh mesh)
  : mesh(std::move(mesh))
{}

Member::~Member() {
  
}

void Member::rotate(glm::vec2 rotation) {

}

void Member::updateRotation(float maxRotation) {

}

void Member::draw() {

}

void Member::addChild(std::unique_ptr<Member> child) {
  children.push_back(std::move(child));
}
