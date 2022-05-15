#include "Node.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

void Node::computeTransforms(highp_dmat4 parentModel) {
  model = I;
  model = translate(model, loc);
  model = scale(model, sca);
  model = model * rotate(I, rot.y, {0, 1, 0});
  model = model * rotate(I, rot.x, {1, 0, 0});
  model = model * rotate(I, rot.z, {0, 0, 1});

  model = parentModel * model;

  for(auto child : children) {
    child->computeTransforms(model);
  }
}
