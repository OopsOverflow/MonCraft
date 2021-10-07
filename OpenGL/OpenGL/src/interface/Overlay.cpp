#include "Overlay.hpp"
#include "gl/ResourceManager.hpp"

Overlay::Overlay()
{
	auto font = ResourceManager::getFont("roboto");
	btn_vsync = ui::Button::makePaneButton("Vsync", font);
	btn_fullscreen = ui::Button::makePaneButton("Fullscreen", font);
	btn_ping = ui::Button::makePaneButton("Ping", font);

	setPadding({ 15, 15 });

	add(btn_vsync.get());
	add(btn_fullscreen.get());
	add(btn_ping.get());

	btn_fullscreen->setAnchorX(ui::Anchor::END);
	btn_ping->setPosition({ 0, 80 });
}

void Overlay::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Component::draw();
}
