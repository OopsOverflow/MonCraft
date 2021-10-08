#include "Box.hpp"

#include <algorithm>

using namespace ui;
using namespace glm;

MAKE_TYPE(Box::Orientation);
const spec_t Box::GAP = MAKE_SPEC("Box::gap", int);
const spec_t Box::ORIENTATION = MAKE_SPEC("Box::orientation", Box::Orientation);

Box::Box()
  : gap(0),
    orientation(Orientation::VERTICAL)
{}

Box::~Box()
{}

void Box::setProperty(prop_t prop) {
  if(prop.spec == Box::GAP) {
    setGap(prop.value->get<int>());
  }
  if(prop.spec == Box::ORIENTATION) {
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
    make_property(Box::GAP, 0),
    make_property(Box::ORIENTATION, Orientation::VERTICAL)
  );

  return style;
}


void Box::pack(size_t index, Component* comp) {
  auto it = cells.begin() + index;
  it = cells.insert(it, std::make_unique<Cell>(comp));
  add(it->get());
}

void Box::pack_start(Component* comp) {
  cells.emplace_back(std::make_unique<Cell>(comp));
  add(cells.back().get());
}

void Box::pack_end(Component* comp) {
  auto it = cells.emplace(cells.begin(), std::make_unique<Cell>(comp));
  add(it->get());
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
  int i = orientation == Orientation::HORIZONTAL ? 0 : 1;
  ivec2 maxSize(0);

  for(auto const& cell : cells) {
    ivec2 pos(0);
    pos[i] = offset;
    cell->setPosition(pos);
    maxSize = max(maxSize, cell->getAbsoluteSize());
    offset += cell->getAbsoluteSize()[i] + gap;
  }

  maxSize[i] = 0;
  for(auto const& cell : cells) {
    cell->setSize(maxSize);
  }
}

void Box::setGap(int gap) {
  this->gap = gap;
}

int Box::getGap() const {
  return gap;
}

void Box::setOrientation(Orientation orientation) {
  if(orientation != this->orientation) {
    this->orientation = orientation;
    updateCells();
  }
}

Box::Orientation Box::getOrientation() const {
  return orientation;
}

Box::Cell::Cell(Component* comp) {
  add(comp);
}

bool Box::Cell::contains(Component* comp) const {
  return children.at(0) == comp;
}
