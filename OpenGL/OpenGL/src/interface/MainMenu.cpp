#include "MainMenu.hpp"
#include "gl/ResourceManager.hpp"

MainMenu::MainMenu()
{
	auto font = ResourceManager::getFont("roboto");

	titlePane = std::make_unique<ui::Pane>();
	moncraftTitle = std::make_unique<ui::Text>("MONCRAFT", font);
	{
		auto img = std::make_unique<ui::Image>();
		playButton = std::make_unique<ui::Button>(move(img), "PLAY", font);
	}

	add(titlePane.get());
	titlePane->add(moncraftTitle.get());
	add(playButton.get());

	titlePane->setAnchorX(ui::Anchor::CENTER);
	titlePane->setAnchorY(ui::Anchor::END);

	setCrop(Crop::CENTER);
}

void MainMenu::draw() {
	if(parent) {
		setSize(parent->getSize());
	}
	Image::draw();
}
