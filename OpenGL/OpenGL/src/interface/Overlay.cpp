#include "Overlay.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;

Overlay::Overlay()
{
	auto font = ResourceManager::getFont("roboto");
	btn_vsync = Button::createPaneButton("Vsync", font);
	btn_fullscreen = Button::createPaneButton("Fullscreen", font);
	btn_ping = Button::createPaneButton("Ping", font);

	setPadding({ 15, 15 });

	add(btn_vsync.get());
	add(btn_fullscreen.get());
	add(btn_ping.get());

	btn_fullscreen->setAnchorX(Anchor::END);
	btn_ping->setPosition({ 0, 80 });
}

void Overlay::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Component::draw();
}
