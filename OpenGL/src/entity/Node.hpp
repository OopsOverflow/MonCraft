#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "util/Bezier.hpp"

/**
 * A node is a geometry point in space. It holds a position, location and scale
 * and can be hierarchical, i.e. can have children, which will apply their
 * transform in their parent's local space.
 */

class Node {

public:
  Node();

  /**
   * read/write parameters of the node. Invalidates the model.
   */
  glm::highp_dvec3 loc;
  glm::highp_dvec3 rot;
  glm::highp_dvec3 sca;

  /**
   * The node model is computed when the root node calls update().
   * /!\ this is readonly
   */
  glm::highp_dmat4 model;

  /**
   * A root node may call update() to compute its model and recursively all of
   * the children models.
   */
  void update();

  /**
   * Adds a Node child to this node.
   * /!\ the child lifetime is not managed by the node and must exceed or equal
   * this node's lifetime.
   */
  void addChild(Node* child);

  void lookAt(glm::highp_dvec3 point);

private:
  void computeTransforms(glm::highp_dmat4 parentModel);
  std::vector<Node*> children;

};
