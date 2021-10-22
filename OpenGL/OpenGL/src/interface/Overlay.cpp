#include "Overlay.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;

Overlay::Overlay()
{
	auto font = ResourceManager::getFont("roboto");
	btn_block = Button::createPaneButton("Change block", font);
	add(btn_block.get());
}

void Overlay::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Component::draw();
}
