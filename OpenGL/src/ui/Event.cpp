#include "Event.hpp"



Event::Event(Type type, glm::ivec2 pos)
  : type(type), pos(pos)
{ }

Event::~Event()
{ }

glm::ivec2 Event::getPosition() const {
  return pos;
}

Event::Type Event::getType() const {
  return type;
}
