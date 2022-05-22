#include "Node.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

using namespace glm;

static const highp_dmat4 I(1.0);

Node::Node()
  : loc(0), rot(0), sca(1), model(I)
{

}

void Node::update() {
  computeTransforms(I);
}

void Node::addChild(Node* child) {
  children.push_back(child);
}

void Node::lookAt(glm::highp_dvec3 point) {
    const glm::highp_dvec3 up = {1., 0., 0.};
    const glm::highp_dvec3 alternativeUp = {0., 0., -1.};
    
    quat rotation;

    if(!(distance(point, glm::highp_dvec3(0.)) > 0.000001))
        rotation = glm::quat(1, 0, 0, 0);

    point = normalize(point);

    if(glm::abs(glm::dot(point, up)) > .999999) {
        // Use alternative up
        if(glm::dot(point, up) > 0.)
          rotation = glm::quatLookAtLH(point, alternativeUp);
        else
          rotation = glm::quatLookAtLH(point, -alternativeUp);
    }
    else {
        rotation = glm::quatLookAtLH(point, up);
    }

    rot = glm::eulerAngles(rotation);

}

void Node::computeTransforms(highp_dmat4 parentModel) {
  model = I;
  model = translate(model, loc);
  model = scale(model, sca);
  quat rotation = quat(rot);
  model = model * (glm::highp_dmat4)mat4_cast(rotation);

  model = parentModel * model;

  for(auto child : children) {
    child->computeTransforms(model);
  }
}
