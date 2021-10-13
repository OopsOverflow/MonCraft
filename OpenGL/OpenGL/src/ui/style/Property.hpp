#pragma once

#include "Specification.hpp"
#include "Value.hpp"
#include <memory>

namespace ui {

struct prop_t {
  spec_t spec;
  std::shared_ptr<const AbstractValue> value;
};

template<typename T>
prop_t make_property(spec_t spec, T val) {
  prop_t prop = {
    spec,
    std::make_unique<Value<T>>(val)
  };

  if(prop.value->getType() != Spec::get(spec).type) {
    throw StyleError(
      "cannot create property with incompatible spec and value: spec '" +
      Spec::get(spec).name +
      "' expects '" +
      Type::getName(Spec::get(spec).type) +
      "', got '" +
      Type::getName(prop.value->getType()) +
      "'"
    );
  }
  return prop;
}

}; // namespace ui
