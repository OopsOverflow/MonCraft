#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

#include "Node.hpp"
#include "../gl/Mesh.hpp"

class Member {

public:
  Member(Mesh mesh);
  virtual ~Member();

  void draw();

  Node node;

protected:
  glm::mat4 geometryModel;
  Mesh mesh;
};
