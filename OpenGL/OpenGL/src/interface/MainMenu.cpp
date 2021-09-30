#include "MainMenu.hpp"
#include "gl/ResourceManager.hpp"

MainMenu::MainMenu()
{
	auto font = ResourceManager::getFont("roboto");

	titlePane = std::make_unique<ui::Pane>();
	moncraftTitle = std::make_unique<ui::Text>("MONCRAFT", font);
	playButton = std::make_unique<ui::Button>("PLAY", font);

	addChild(titlePane.get());
	titlePane->addChild(moncraftTitle.get());
	addChild(playButton.get());

	titlePane->setAnchorX(ui::Anchor::CENTER);
	titlePane->setAnchorY(ui::Anchor::END);
}

void MainMenu::draw() {
	if(parent) {
		std::cout << "hello" << std::endl;
		setSize(parent->getSize());
	}
	Component::draw();
}
