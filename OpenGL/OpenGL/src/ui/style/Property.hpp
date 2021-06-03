#pragma once

#include "Specification.hpp"
#include "Value.hpp"

namespace ui {

struct prop_t {
  spec_t spec;
  std::unique_ptr<AbstractValue> value;
};

template<typename T>
prop_t make_property(spec_t spec, T val) {
  prop_t prop = {
    spec,
    std::make_unique<Value<T>>(val)
  };

  if(prop.value->getType() != Specification::get(spec).type) {
    throw StyleError(
      "cannot create property with incompatible spec and value: spec '" +
      Specification::get(spec).name +
      "' expects '" +
      Type::getName(Specification::get(spec).type) +
      "', got '" +
      Type::getName(prop.value->getType()) +
      "'"
    );
  }
  return prop;
}

}; // namespace ui
