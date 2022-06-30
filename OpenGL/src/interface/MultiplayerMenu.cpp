#include "MultiplayerMenu.hpp"
#include "gl/ResourceManager.hpp"



MultiplayerMenu::MultiplayerMenu() : Image({ 16, 1194 }, { 16, 16 })
{
	setCrop(Crop::REPEAT);
	auto font = ResourceManager::getFont("roboto");

	serverAddrInput = ui::Input::create("pi.thissma.fr", font);
	portInput = ui::Input::create("55000", font);
	serverAddrImg = ui::Image::create(glm::ivec2(0, 1083), glm::ivec2(429, 92));
	portImg = ui::Image::create(glm::ivec2(0, 1083), glm::ivec2(429, 92));



}

void MultiplayerMenu::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Image::draw();
}
