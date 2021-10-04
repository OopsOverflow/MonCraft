#include "Style.hpp"

#include "../Component.hpp"

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

void Style::apply(Component* comp) const {
  for(auto const& pair : props) {
    comp->setStyle(pair.second);
  }
}

void Style::revert(Component* comp) const {
  if(parent) {
    for(auto const& pair : props) {
      auto prop = parent->getProperty(comp, pair.first);
      if(prop.value != nullptr) comp->setStyle(prop);
    }
  }
  else {
    auto fallback = comp->getDefaultStyle();
    for(auto const& pair : props) {
      auto prop = fallback->getProperty(pair.first);
      if(prop.value != nullptr) comp->setStyle(prop);
    }
  }
}

void Style::applyAll(Component* comp) const {
  if(parent) parent->applyAll(comp);
  apply(comp);
}

bool Style::hasProperty(spec_t spec) const {
  if(parent) {
    if(parent->hasProperty(spec)) return true;
  }
  return props.find(spec) != props.cend();
}

prop_t Style::getProperty(spec_t spec) const {
  auto it = props.find(spec);
  if(it != props.cend()) return it->second;
  else if(parent) return parent->getProperty(spec);
  else return prop_t{ spec, nullptr };
}

prop_t Style::getProperty(Component* comp, spec_t spec) const {
  auto it = props.find(spec);
  if(it != props.cend()) return it->second;
  else if(parent) return parent->getProperty(comp, spec);
  else return comp->getDefaultStyle()->getProperty(spec);
}

style_t Style::make_style(style_const_t parent) {
  return std::make_shared<Style>(parent);
}
