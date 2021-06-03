#pragma once

#include "Component.hpp"
#include "gl/Shader.hpp"
#include "Event.hpp"

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
