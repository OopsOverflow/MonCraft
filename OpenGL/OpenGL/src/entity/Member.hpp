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
  Member();
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

  virtual std::unique_ptr<Mesh> createMesh() = 0;

private:
  std::unique_ptr<Mesh> mesh;
};
