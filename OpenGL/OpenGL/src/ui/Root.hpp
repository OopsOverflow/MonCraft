#pragma once

#include "Component.hpp"

namespace ui {

class Root : public Component {

public:
  Root(glm::ivec2 size);

  void render();

  void setSize(glm::ivec2);
  glm::ivec2 getSize() const override;

private:
  glm::ivec2 size;

};

}; // namespace ui
