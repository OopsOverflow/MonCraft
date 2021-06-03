#include "Type.hpp"
#include "debug/Debug.hpp"

using namespace ui;

type_t Type::registerType() {
  static type_t type = 0;
  return type++;
}

// some predefined types

#include <glm/glm.hpp>

MAKE_TYPE(int);
MAKE_TYPE(double);
MAKE_TYPE(std::string);
MAKE_TYPE(glm::ivec2);

// ----
