#include "Box.hpp"

#include <algorithm>

using namespace ui;
using namespace glm;

const spec_t Box::GAP = MAKE_SPEC("Box::gap", int);

Box::Box()
  : gap(0)
{}

Box::~Box()
{}

void Box::setProperty(prop_t prop) {
  if(prop.spec == Box::GAP) {
    setGap(prop.value->get<int>());
  }
  else {
    Component::setProperty(prop);
  }
}

prop_t Box::getProperty(spec_t spec) const {
  if(spec == Box::GAP) {
    return make_property(spec, getGap());
  }
  else {
    return Component::getProperty(spec);
  }
}

style_const_t Box::getDefaultStyle() const {
  static style_const_t style = Style::make_style(
    Component::getDefaultStyle(),
    make_property(Box::GAP, 0)
  );

  return style;
}


void Box::pack(Component* comp) {
  cells.emplace_back(std::make_unique<Cell>(comp));
  add(cells.back().get());
}

void Box::unpack(Component* comp) {
  for(auto it = cells.cbegin(); it != cells.cend(); ) {
    if((*it)->contains(comp)) it = cells.erase(it);
    else ++it;
  }
}

void Box::draw() {
  updateCells();

  Component::draw();
}

void Box::updateCells() {
  int offset = 0;
  std::vector<int> offsets(cells.size());

  for(auto const& cell : cells) {
    cell->setPosition(ivec2(0, offset));
    offset += cell->getAbsoluteSize().y + gap;
  }
}

void Box::setGap(int gap) {
  this->gap = gap;
}

int Box::getGap() const {
  return gap;
}

Box::Cell::Cell(Component* comp) {
  add(comp);
}

bool Box::Cell::contains(Component* comp) const {
  return children.at(0) == comp;
}
