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

void Style::apply(Component* comp) const {
  for(auto const& pair : props) {
    comp->setStyle(pair.second);
  }
}

void Style::revert(Component* comp) const {
  if(!parent) return;
  for(auto const& pair : props) {
    auto it = parent->props.find(pair.first);
    if(it != parent->props.end()) {
      comp->setStyle(it->second);
    }
    else {
      std::cout
        << "[WARN] cannot revert property: '"
        << Specification::get(pair.first).name
        << "'" << std::endl;
    }
  }
}

void Style::applyAll(Component* comp) const {
  if(parent) parent->applyAll(comp);
  apply(comp);
}

style_t Style::make_style(style_const_t parent) {
  return std::make_shared<Style>(parent);
}
