#include "Type.hpp"
#include "StyleError.hpp"
#include <string>

using namespace ui;

std::vector<std::string>& Type::getNames() {
  static std::vector<std::string> names{};
  return names;
}

type_t Type::registerType(std::string name) {
  static type_t type = 0;
  getNames().push_back(name);
  return type++;
}

void Type::assertTypeValid(type_t type) {
  if(type >= getNames().size())
    throw StyleError("invalid type: " + std::to_string(type));
}

std::string Type::getName(type_t type) {
  assertTypeValid(type);
  return getNames().at(type);
}

// some predefined types

#include <glm/glm.hpp>

MAKE_TYPE(bool);
MAKE_TYPE(int);
MAKE_TYPE(float);
MAKE_TYPE(std::string);
MAKE_TYPE(glm::ivec2);
MAKE_TYPE(glm::ivec3);
MAKE_TYPE(glm::ivec4);
MAKE_TYPE(glm::vec2);
MAKE_TYPE(glm::vec3);
MAKE_TYPE(glm::vec4);

// ----
