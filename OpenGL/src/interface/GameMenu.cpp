#include "GameMenu.hpp"
#include "widgets/MonCraftButton.hpp"
#include "ui/Box.hpp"
using namespace ui;

GameMenu::GameMenu()
{

	auto buttons = Box::create();

	continueButton = MonCraftButton::create("Continuer");
	parameterButton = MonCraftButton::create("Paramètres");
	quitButton = MonCraftButton::create("Quitter");

	buttons->setAnchorX(Anchor::CENTER);
	buttons->setAnchorY(Anchor::CENTER);
	buttons->setGap(10);

	buttons->pack_start(continueButton.get());
	#ifndef EMSCRIPTEN
		buttons->pack_start(parameterButton.get());
		buttons->pack_start(quitButton.get());
	#endif

	add(move(buttons));

}

std::unique_ptr<GameMenu> GameMenu::create() {
	auto menu = std::unique_ptr<GameMenu>(new GameMenu());
	menu->initialize();
	return menu;
}

bool GameMenu::onMousePressed(glm::ivec2 pos) {
    return true;
}

