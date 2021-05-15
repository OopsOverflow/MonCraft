#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

#include "Node.hpp"
#include "../gl/Mesh.hpp"

/**
 * An entity's single body part.
 */

class Member {

public:
  /**
   * Creates a member from it's mesh.
   */
  Member(Mesh mesh);
  virtual ~Member();

  /**
   * Draws the member.
   */
  void draw();

  /**
   * The node holds the loc/rot/scale of this member.
   */
  Node node;

protected:
  glm::mat4 geometryModel;
  Mesh mesh;
};
