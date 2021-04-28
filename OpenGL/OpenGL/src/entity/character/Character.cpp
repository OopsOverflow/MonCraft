#include "Character.hpp"

using namespace glm;

Character::Character(vec3 pos) {
  node.loc = pos;
  rootNode.sca = vec3(.1f);
  rootNode.rot.y = glm::pi<float>();
  headNode.loc = {0, 6, 0};
  node.addChild(&rootNode);
  rootNode.addChild(&headNode);
  rootNode.addChild(&chest.node);
  rootNode.addChild(&head.node);
  chest.node.addChild(&l_arm.node);
  chest.node.addChild(&r_arm.node);
  chest.node.addChild(&l_leg.node);
  chest.node.addChild(&r_leg.node);

  tex = texLoader.loadTexture("Character");
}

Character::~Character() {}

#include "../../debug/Debug.hpp"

void Character::update(float dt) {
  // smooth head rot with constant speed
  {
    float speed = 5;
    auto dist = headNode.rot - head.node.rot;
    if(dist != vec3(0)) {
      auto delta = normalize(dist) * speed * dt;
      if(any(greaterThan(abs(delta), abs(dist)))) {
        head.node.rot = headNode.rot;
      }
      else {
        head.node.rot += delta;
      }
    }
  }

  Entity::update(dt);
}

void Character::render() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);
  chest.draw();
  head.draw();
  l_arm.draw();
  r_arm.draw();
  l_leg.draw();
  r_leg.draw();
  glBindTexture(GL_TEXTURE_2D, 0);
}
