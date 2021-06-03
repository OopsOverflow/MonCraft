#pragma once

#include <stdexcept>
#include <vector>
#include <unordered_map>
#include "debug/Debug.hpp"
#include "Type.hpp"
#include "StyleError.hpp"

namespace ui
{

template<typename T>
class Value;

class AbstractValue {
public:
  virtual ~AbstractValue() { }

  template<typename T>
  T get() const {
    if(Type::getType<T>() == type) {
      return static_cast<const Value<T>*>(this)->get();
    }
    throw StyleError("cannot get property value of wrong type");
  }

  template<typename T>
  void set(T val) {
    if(Type::getType<T>() == type) {
      static_cast<Value<T>*>(this)->set(val);
    }
    throw StyleError("cannot set property value of wrong type");
  }

  type_t getType() const {
    return type;
  }

protected:
  AbstractValue(type_t type) : type(type) {}

private:
  type_t type;
};

template<typename T>
class Value : public AbstractValue {
public:
  Value(T val)
  : AbstractValue(Type::getType<T>()),
    val(val)
  { }

  T get() const {
    return val;
  }

  void set(T newVal) {
    val = newVal;
  }

private:
  T val;
};

}; // namespace ui
