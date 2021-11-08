#include "Specification.hpp"

#include <memory>

#include "ui/style/StyleError.hpp"
#include "ui/style/Type.hpp"

using namespace ui;

Spec::map_t& Spec::getMap() {
  static map_t propLookup{};
  return propLookup;
}
Spec::pool_t& Spec::getPool() {
  static pool_t specPool{};
  return specPool;
}

Spec::Spec(std::string name, type_t type, bool inherit)
  : name(name), type(type), inherit(inherit)
{ }

spec_t Spec::create(std::string name, type_t type, bool inherit) {
  auto res = getMap().emplace(name, getPool().size());
  if(res.second) {
    getPool().emplace_back(Spec(name, type, inherit));
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

Spec const& Spec::get(spec_t spec) {
  assertSpecValid(spec);
  return getPool().at(spec);
}

void Spec::assertSpecValid(spec_t spec) {
  if(spec >= getPool().size()) {
    throw StyleError(
      "invalid property spec: '" +
      std::to_string(spec) +
      "'"
    );
  }
}
