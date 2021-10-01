#include "GameMenu.hpp"
#include "gl/ResourceManager.hpp"

GameMenu::GameMenu()
{
	auto font = ResourceManager::getFont("roboto");
	titlePane = std::make_unique<ui::Pane>();
	pauseTitle = std::make_unique<ui::Text>("Pause", font);
	playButton = std::make_unique<ui::Button>("Resume", font);
	quitButton = std::make_unique<ui::Button>("Quit", font);

	addChild(titlePane.get());
	titlePane->addChild(pauseTitle.get());
	addChild(playButton.get());
	addChild(quitButton.get());

	titlePane->setAnchorX(ui::Anchor::CENTER);
	titlePane->setAnchorY(ui::Anchor::END);

	playButton->setAnchorX(ui::Anchor::CENTER);
	playButton->setPosition(glm::ivec2(0, -185)); // TODO: implement a box container

	quitButton->setAnchorX(ui::Anchor::CENTER);
	playButton->setPosition(glm::ivec2(0, -385)); // TODO: implement a box container

}

void GameMenu::draw() {
	setSize(parent->getSize());
	Component::draw();

}