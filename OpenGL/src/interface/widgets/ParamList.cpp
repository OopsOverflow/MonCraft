#include "ParamList.hpp"

#include "gl/ResourceManager.hpp"
#include "ui/Text.hpp"

using namespace ui;

ParamList::ParamList()
  : font(ResourceManager::getFont("roboto")),
    lastRow(0)
{
  grid = Grid::create();
	grid->setGap({20, 20});
  setPadding({ 0, 20 });
  add(grid.get());
}

void ParamList::addLine(std::string name, std::unique_ptr<Component> comp) {
  grid->pack({0, lastRow}, Text::create(name, font));
  grid->pack({1, lastRow}, move(comp));
  lastRow++;
}

void ParamList::addLine(std::string name, Component* comp) {
  grid->pack({0, lastRow}, Text::create(name, font));
  grid->pack({1, lastRow}, comp);
  lastRow++;
}
