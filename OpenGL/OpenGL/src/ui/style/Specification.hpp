#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "Type.hpp"

namespace ui {

using spec_t = size_t;

class Specification {
public:
  static spec_t create(std::string name, type_t type);
  static Specification const& get(spec_t spec);

  std::string name;
  type_t type;

private:
  Specification(std::string name, type_t type);
  static void assertSpecValid(spec_t spec);

  using map_t = std::unordered_map<std::string, spec_t>;
  using pool_t = std::vector<Specification>;

  static map_t& getMap();
  static pool_t& getPool();
};

#define MAKE_SPEC(N, T) \
  Specification::create(N, Type::getType<T>());

}; // namespace ui
