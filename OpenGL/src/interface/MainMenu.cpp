#include "MainMenu.hpp"

#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "interface/widgets/MonCraftButton.hpp"
#include "ui/Component.hpp"
#include "ui/Box.hpp"
#include "ui/Image.hpp"
#include "ui/style/Type.hpp"

using namespace ui;

MainMenu::MainMenu()
{
	background = Image::create({0, 0}, {1920, 1080});
	auto title = Image::create({0, 1083}, {429, 92});
	auto buttons = Box::create();

	background->setCrop(Crop::CENTER);
	background->setMinFilter(GL_LINEAR);
	background->setMagFilter(GL_LINEAR);

	background->setPadding(glm::ivec2(15, 15));

	singleplayerButton = MonCraftButton::create("Solo");
	multiplayerButton = MonCraftButton::create("Multijoueur");
	parameterButton = MonCraftButton::create("Paramètres");
	quitButton = MonCraftButton::create("Quitter");

	buttons->setAnchorX(Anchor::CENTER);
	buttons->setAnchorY(Anchor::CENTER);
	buttons->setGap(10);

	buttons->pack_start(singleplayerButton.get());
	buttons->pack_start(multiplayerButton.get());
	#ifndef EMSCRIPTEN
		buttons->pack_start(parameterButton.get());
		buttons->pack_start(quitButton.get());
	#endif

	title->setAnchorX(Anchor::CENTER);
	title->setAnchorY(Anchor::END);
	title->setSize(glm::vec2(429, 92) * 1.5f);
	title->setCrop(Crop::CENTER);
	title->setMinFilter(GL_LINEAR);
	title->setMagFilter(GL_LINEAR);

	background->add(move(title));
	background->add(move(buttons));
	add(background.get());
}

void MainMenu::draw() {
	if(parent) {
		background->setSize(parent->getSize());
	}
	Component::draw();
}

std::unique_ptr<MainMenu> MainMenu::create() {
	auto menu = std::unique_ptr<MainMenu>(new MainMenu());
	menu->initialize();
	return menu;
}
