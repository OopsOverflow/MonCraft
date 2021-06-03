#pragma once

#include <glm/glm.hpp>


class Event {
public:

  enum class Type { PRESS, RELEASE, MOVE };

  Event(Type type, glm::ivec2 pos);
  virtual ~Event();

  Type getType() const;
  glm::ivec2 getPosition() const;

private:
  Type type;
  glm::ivec2 pos;
};
