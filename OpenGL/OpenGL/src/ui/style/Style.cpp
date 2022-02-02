#include "Style.hpp"

#include <utility>

#include "ui/style/Property.hpp"
#include "ui/style/Type.hpp"

using namespace ui;

Style::Style()
: parent(nullptr)
{}

Style::Style(style_const_t parent)
  : parent(std::move(parent))
{}

Style::Style(std::initializer_list<prop_t> list, style_const_t parent)
  : parent(parent)
{
  for(prop_t prop : list) {
    props.emplace(prop.spec, prop);
  }
}

void Style::set(prop_t prop) {
  props[prop.spec] = std::move(prop);
}

void Style::setParent(style_const_t parent) {
  this->parent = parent;
}

style_const_t Style::getParent() const {
  return parent;
}

prop_t Style::get(spec_t spec) const {
  auto it = props.find(spec);
  if(it != props.cend()) return it->second;
  else if(parent) return parent->get(spec);
  else return prop_t{ spec, nullptr };
}

bool Style::has(spec_t spec) const {
  if(parent) {
    if(parent->has(spec)) return true;
  }
  return props.find(spec) != props.cend();
}

style_t Style::make_style(style_const_t parent) {
  return std::make_shared<Style>(parent);
}
