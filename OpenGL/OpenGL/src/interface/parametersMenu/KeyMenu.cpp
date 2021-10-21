#include "KeyMenu.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;
using namespace glm;

KeyMenu::KeyMenu()
	: Box()
{}

void KeyMenu::draw() {
	Box::draw();
}

std::unique_ptr<Box> KeyMenu::create() {
	auto font = ResourceManager::getFont("roboto");
	auto btn = new KeyMenu();
	btn->initialize();
	return std::unique_ptr<Box>(btn);
}

