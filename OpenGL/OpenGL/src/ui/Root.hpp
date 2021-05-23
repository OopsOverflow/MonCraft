#pragma once

#include "Component.hpp"
#include "gl/Shader.hpp"

namespace ui {

class Root : public Component {

public:
  Root(glm::ivec2 size);

  void render();

private:
  std::vector<Shader*> shaders;

};

}; // namespace ui
