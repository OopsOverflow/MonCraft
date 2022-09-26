#include "CharacterMesh.hpp"

using namespace glm;

CharacterMesh::CharacterMesh()
    : Character()
{
  bodyNode.loc = getPosition(); // is this needed?
  rootNode.sca = vec3(1.85f / 32.f); // steve is 1.85 blocks high, 32 pixels high
  rootNode.rot.y = glm::pi<float>();
  rootNode.loc.y = 9.5 / 32. * 1.85;
  headNode.loc = {0, 6, 0};
  bodyNode.addChild(&rootNode);
  rootNode.addChild(&headNode);
  rootNode.addChild(&chest.node);
  rootNode.addChild(&head.node);
  chest.node.addChild(&l_arm.node);
  chest.node.addChild(&r_arm.node);
  chest.node.addChild(&l_leg.node);
  chest.node.addChild(&r_leg.node);
  animState = 0;

}

CharacterMesh::~CharacterMesh() {}

void CharacterMesh::render() {
  chest.draw();
  head.draw();
  l_arm.draw();
  r_arm.draw();
  l_leg.draw();
  r_leg.draw();
}

void CharacterMesh::update(uint32_t dt) {
  Entity::update(dt);

  if(breaked) {
    r_arm.anim->setAnimation(Animation::Break);
    breaked = false;
  }
   

  // smooth head rot with constant speed
  {
    float speed;
    glm::vec3 dist;
    glm::highp_dvec3 target;
    if(!dab) {
      target = headNode.rot;
      speed = 5;
    }
    else {
      target = radians(highp_dvec3(35., -40., 0.));
      speed = 15;
    }
      
    
    dist = vec3(target - head.node.rot);
    if(dist != vec3(0)) {
      auto delta = normalize(dist) * (speed * dt * 0.001f);
      if(any(greaterThan(abs(delta), abs(dist)))) {
        head.node.rot = target;
      }
      else {
        head.node.rot += delta;
      }
    }
  }

  r_arm.animate(dt);
  l_arm.animate(dt);
  l_leg.animate(dt);
  r_leg.animate(dt);

  // // walk animation
    if(state == State::Walking) {
        r_arm.anim->setAnimation(Animation::Walk);
        l_arm.anim->setAnimation(Animation::Walk);
        r_leg.anim->setAnimation(Animation::Walk);
        l_leg.anim->setAnimation(Animation::Walk);
    }
    else {
      r_arm.anim->setAnimation(Animation::Idle);
      l_arm.anim->setAnimation(Animation::Idle);
      r_leg.anim->setAnimation(Animation::Idle);
      l_leg.anim->setAnimation(Animation::Idle);

    }

    if(dab) {
      r_arm.anim->setAnimation(Animation::Dab);
      l_arm.anim->setAnimation(Animation::Dab);
    }

}