#pragma once

#include <stdexcept>
#include <vector>
#include <unordered_map>
#include "debug/Debug.hpp"

namespace ui {

using type_t = size_t;

class Type {
public:
  Type() = delete;

  template<typename T>
  static type_t getType();

private:
  static type_t registerType();
};

#define MAKE_TYPE(T)                           \
  template <>                                  \
  type_t Type::getType<T>() {                  \
    static const type_t type = registerType(); \
    return type;                               \
  }

}; // namespace ui
