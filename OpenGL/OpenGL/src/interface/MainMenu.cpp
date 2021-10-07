#include "MainMenu.hpp"
#include "gl/ResourceManager.hpp"

MainMenu::MainMenu() : Image({0, 0}, {1920, 1080})
{
	auto font = ResourceManager::getFont("roboto");

	titlePane = std::make_unique<ui::Pane>();
	moncraftTitle = std::make_unique<ui::Text>("MONCRAFT", font);
	{
		auto img = std::make_unique<ui::Image>();
		playButton = std::make_unique<ui::Button>(move(img), "PLAY", font);
	}
	playButton->setHoverStyle(ui::make_property(ui::Text::COLOR, glm::vec4(1.f, 1.f, 0.f, 1.f)));

	add(titlePane.get());
	titlePane->add(moncraftTitle.get());
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
