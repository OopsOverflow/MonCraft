#include "GraphicsMenu.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;
using namespace glm;

GraphicsMenu::GraphicsMenu()
	: Box()
{}

void GraphicsMenu::draw() {
	Box::draw();
}

std::unique_ptr<Box> GraphicsMenu::create() {
	auto font = ResourceManager::getFont("roboto");
	auto btn = new GraphicsMenu();
	btn->initialize();
	return std::unique_ptr<Box>(btn);
}

