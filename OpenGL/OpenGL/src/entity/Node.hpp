#pragma once

#include <glm/glm.hpp>
#include <vector>


class Node {

public:
  Node();

  glm::vec3 loc;
  glm::vec3 rot;
  glm::vec3 sca;
  glm::mat4 model;

  void update();
  void addChild(Node* child);

private:
  void computeTransforms(glm::mat4 parentModel);
  std::vector<Node*> children;
};
