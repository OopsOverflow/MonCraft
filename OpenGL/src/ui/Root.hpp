#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ui/Component.hpp"
#include "ui/Event.hpp"

class Shader;

namespace ui {

class Root : public Component {

public:
  Root(glm::ivec2 size);

  void addEvent(Event evt);
  void update();
  void render();

private:
  std::vector<Shader*> shaders;
  std::vector<Event> events;
};

}; // namespace ui
