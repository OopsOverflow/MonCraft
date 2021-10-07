#include "MainMenu.hpp"
#include "gl/ResourceManager.hpp"

MainMenu::MainMenu() : Image({0, 0}, {1920, 1080})
{
	auto font = ResourceManager::getFont("roboto");

	MoncraftTitle = std::make_unique<ui::Image>(glm::ivec2(0, 1081), glm::ivec2(438, 100));
	playButton = std::make_unique<ui::Button>("PLAY", font);
	
	add(MoncraftTitle.get());
	add(playButton.get());

	MoncraftTitle->setAnchorX(ui::Anchor::CENTER);
	MoncraftTitle->setAnchorY(ui::Anchor::END);

	MoncraftTitle->setSize({ 1.5*438,1.5*100 });
	MoncraftTitle->setCrop(Crop::CENTER);

	setCrop(Crop::CENTER);
}

void MainMenu::draw() {
	if(parent) {
		setSize(parent->getSize());
	}
	Image::draw();
}
