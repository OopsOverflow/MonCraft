#include "Grid.hpp"

#include <utility>
#include <glm/glm.hpp>

#include "ui/Component.hpp"
#include "ui/style/Type.hpp"
#include "ui/style/Value.hpp"

using namespace ui;
using namespace glm;

const spec_t Grid::GAP = MAKE_SPEC("Grid::gap", glm::ivec2);

Grid::Grid()
  : cells(0)
{}

std::unique_ptr<Grid> Grid::create() {
  auto comp = std::unique_ptr<Grid>(new Grid());
  comp->initialize();
  return comp;
}

void Grid::setProperty(prop_t prop) {
  if(prop.spec == Grid::GAP) {
    setGap(prop.value->get<glm::ivec2>());
  }
  else {
    Component::setProperty(prop);
  }
}

prop_t Grid::getProperty(spec_t spec) const {
  if(spec == Grid::GAP) {
    return make_prop(spec, getGap());
  }
  else {
    return Component::getProperty(spec);
  }
}

style_const_t Grid::getDefaultStyle() const {
  static style_const_t style = Style::make_style(
    Component::getDefaultStyle(),
    Grid::GAP, glm::ivec2(0, 0)
  );

  return style;
}

void Grid::pack(glm::ivec2 pos, std::shared_ptr<Component> comp) {
  ensureCellExists(pos);
  if(cells.at(pos) == nullptr) {
    cells.at(pos) = Cell::create(move(comp));
    add(cells.at(pos));
  }
  else throw std::runtime_error("grid cell already occupied");
}

void Grid::draw() {
  updateCells();

  Component::draw();
}

void Grid::updateCells() {
  ivec2 offset(0);
  auto size = cells.size();
  std::vector<int> colSizes(size.x), rowSizes(size.y);
  ivec2 pos(0);

  // compute row & column sizes
  for (pos.x = 0; pos.x < size.x; pos.x++) {
    for (pos.y = 0; pos.y < size.y; pos.y++) {
      auto const& cell = cells.at(pos);
      if(cell != nullptr) {
        auto cellSize = cell->getAbsoluteSize();
        colSizes[pos.x] = max(colSizes[pos.x], cellSize.x);
        rowSizes[pos.y] = max(rowSizes[pos.y], cellSize.y);
      }
    }
  }

  for (pos.x = 0; pos.x < size.x; pos.x++) {
    for (pos.y = 0; pos.y < size.y; pos.y++) {
      auto const& cell = cells.at(pos);
      if(cell != nullptr) {
        cell->setPosition(offset);
        cell->setSize({colSizes[pos.x], rowSizes[pos.y]});
      }
      offset.y += getGap().y + rowSizes[pos.y];
    }
    offset.x += getGap().x + colSizes[pos.x];
    offset.y = 0;
  }
}

void Grid::ensureCellExists(glm::ivec2 pos) {
  auto diff = glm::max(pos + 1 - cells.size(), 0);

  if(diff.x > 0 || diff.y > 0) {
    cells.grow(diff);
  }
}

void Grid::setGap(glm::ivec2 gap) {
  setStyle(GAP, gap);
}

glm::ivec2 Grid::getGap() const {
  return getStyle<glm::ivec2>(GAP);
}

Grid::Cell::Cell() {}

std::unique_ptr<Grid::Cell> Grid::Cell::create(std::shared_ptr<Component> comp) {
  auto cell = std::unique_ptr<Cell>(new Cell());
  cell->add(move(comp));
  return cell;
}

bool Grid::Cell::contains(Component* comp) const {
  return children.at(0).get() == comp;
}
