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

  static std::string getName(type_t type);

private:
  static type_t registerType(std::string name);
  static void assertTypeValid(type_t type);
  static std::vector<std::string>& getNames();
};

#define MAKE_TYPE(T)                             \
  template <>                                    \
  type_t Type::getType<T>() {                    \
    static const type_t type = registerType(#T); \
    return type;                                 \
  }

}; // namespace ui
