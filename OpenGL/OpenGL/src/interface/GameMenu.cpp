#include "GameMenu.hpp"
#include "gl/ResourceManager.hpp"

GameMenu::GameMenu()
{
	auto font = ResourceManager::getFont("roboto");
	titlePane = std::make_unique<ui::Pane>();
	pauseTitle = std::make_unique<ui::Text>("Pause", font);
	playButton = ui::Button::makePaneButton("Resume", font);
	quitButton = ui::Button::makePaneButton("Quit", font);

	add(titlePane.get());
	titlePane->add(pauseTitle.get());
	add(playButton.get());
	add(quitButton.get());

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
