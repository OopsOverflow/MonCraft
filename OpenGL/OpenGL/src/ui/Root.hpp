#pragma once

#include "Component.hpp"
#include "gl/Shader.hpp"

namespace ui {

class Root : public Component {

public:
  Root(glm::ivec2 size);

  void render();

  void setSize(glm::ivec2);
  glm::ivec2 getSize() const override;

private:
  glm::ivec2 size;
  std::vector<Shader*> shaders;

};

}; // namespace ui
