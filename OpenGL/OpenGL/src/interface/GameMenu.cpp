#include "GameMenu.hpp"

#include <glm/glm.hpp>

#include "gl/ResourceManager.hpp"
#include "ui/style/Type.hpp"

using namespace ui;

GameMenu::GameMenu()
{
	auto font = ResourceManager::getFont("roboto");
	titlePane = Pane::create();
	pauseTitle = Text::create("Pause", font);
	playButton = Button::createPaneButton("Resume", font);
	quitButton = Button::createPaneButton("Quit", font);

	add(titlePane.get());
	titlePane->add(pauseTitle.get());
	add(playButton.get());
	add(quitButton.get());

	titlePane->setAnchorX(Anchor::CENTER);
	titlePane->setAnchorY(Anchor::END);

	playButton->setAnchorX(Anchor::CENTER);
	playButton->setPosition(glm::ivec2(0, -185)); // TODO: implement a box container

	quitButton->setAnchorX(Anchor::CENTER);
	playButton->setPosition(glm::ivec2(0, -385)); // TODO: implement a box container

}

void GameMenu::draw() {
	setSize(parent->getSize());
	Component::draw();

}
