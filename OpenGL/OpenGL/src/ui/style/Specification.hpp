#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "ui/style/Type.hpp"

namespace ui {

using spec_t = size_t;

class Spec {
public:
  static spec_t create(std::string name, type_t type, bool inherit = false);
  static Spec const& get(spec_t spec);

  std::string name;
  type_t type;
  bool inherit;

private:
  Spec(std::string name, type_t type, bool inherit);
  static void assertSpecValid(spec_t spec);

  using map_t = std::unordered_map<std::string, spec_t>;
  using pool_t = std::vector<Spec>;

  static map_t& getMap();
  static pool_t& getPool();
};

#define MAKE_SPEC(N, T) \
  Spec::create(N, Type::getType<T>());

#define MAKE_SPEC_INHERIT(N, T) \
  Spec::create(N, Type::getType<T>(), true);

}; // namespace ui
