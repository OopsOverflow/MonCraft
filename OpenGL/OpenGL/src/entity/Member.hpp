#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "../gl/Mesh.hpp"

class Member {

public:
  Member(Mesh mesh);
  virtual ~Member();

  void rotate(glm::vec2 rotation); // update target rotation
  void updateRotation(float maxRotation);
  void draw();
  void addChild(std::unique_ptr<Member> child);

private:
  glm::mat4 localModel;
  glm::vec2 targetRot;
  glm::vec2 currentRot;
  Mesh mesh;

  std::vector<std::unique_ptr<Member>> children;
};
