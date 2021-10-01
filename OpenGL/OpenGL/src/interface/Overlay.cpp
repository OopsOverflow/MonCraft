#include "Overlay.hpp"
#include "gl/ResourceManager.hpp"

Overlay::Overlay()
{
	auto font = ResourceManager::getFont("roboto");
	btn_vsync = std::make_unique<ui::Button>("Vsync", font);
	btn_fullscreen = std::make_unique<ui::Button>("Fullscreen", font);
	btn_ping = std::make_unique<ui::Button>("Ping", font);

	addChild(btn_vsync.get());
	addChild(btn_fullscreen.get());
	addChild(btn_ping.get());

	btn_vsync->setPadding({ 15, 10 });

	btn_fullscreen->setAnchorX(ui::Anchor::END);
	btn_fullscreen->setPadding({ 15, 10 });

	btn_ping->setPosition({ 0, 80 }); // TODO: implement a box container
	btn_fullscreen->setPadding({ 15, 10 });

}

void Overlay::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Component::draw();
}




