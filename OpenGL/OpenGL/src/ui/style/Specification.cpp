#include "Specification.hpp"
#include "StyleError.hpp"

using namespace ui;

Specification::map_t& Specification::getMap() {
  static map_t propLookup{};
  return propLookup;
}
Specification::pool_t& Specification::getPool() {
  static pool_t specPool{};
  return specPool;
}

Specification::Specification(std::string name, type_t type)
  : name(name), type(type)
{ }

spec_t Specification::create(std::string name, type_t type) {
  auto res = getMap().emplace(name, getPool().size());
  if(res.second) {
    getPool().emplace_back(Specification(name, type));
    return getPool().size() - 1;
  }
  else if(getPool().at(res.first->second).type == type) {
    return res.first->second;
  }
  else {
    throw StyleError(
      "redefined spec with different type: '" +
      name +
      "' was previously defined with type '" +
      Type::getName(getPool().at(res.first->second).type) +
      "', tried to redefine it with type '" +
      Type::getName(type) +
      "'"
    );
  }
}

Specification const& Specification::get(spec_t spec) {
  assertSpecValid(spec);
  return getPool().at(spec);
}

void Specification::assertSpecValid(spec_t spec) {
  if(spec >= getPool().size()) {
    throw StyleError(
      "invalid property spec: '" +
      std::to_string(spec) +
      "'"
    );
  }
}
