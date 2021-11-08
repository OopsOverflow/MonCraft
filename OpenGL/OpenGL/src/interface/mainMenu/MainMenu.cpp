#include "MainMenu.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "interface/mainMenu/MainMenuButton.hpp"
#include "ui/Component.hpp"
#include "ui/style/Type.hpp"

using namespace ui;

MainMenu::MainMenu() : Image({0, 0}, {1920, 1080})
{
	setCrop(Crop::CENTER);
	setMinFilter(GL_LINEAR);
	setMagFilter(GL_LINEAR);
	setPadding(glm::ivec2(15, 15));

	moncraftTitle = Image::create(glm::ivec2(0, 1083), glm::ivec2(429, 92));
	buttonPannel = Box::create();

	singleplayerButton = MainMenuButton::create("Solo");
	multiplayerButton = MainMenuButton::create("Multijoueur");
	parameterButton = MainMenuButton::create("Paramètres");
	quitButton = MainMenuButton::create("Quitter");

	add(moncraftTitle.get());
	add(buttonPannel.get());

	buttonPannel->setAnchorX(Anchor::CENTER);
	buttonPannel->setAnchorY(Anchor::CENTER);
	buttonPannel->setGap(10);

	buttonPannel->pack_start(singleplayerButton.get());
	buttonPannel->pack_start(multiplayerButton.get());
	#ifndef EMSCRIPTEN
		buttonPannel->pack_start(parameterButton.get());
		buttonPannel->pack_start(quitButton.get());
	#endif

	moncraftTitle->setAnchorX(Anchor::CENTER);
	moncraftTitle->setAnchorY(Anchor::END);

	moncraftTitle->setSize({ 1.5*429,1.5*92 });
	moncraftTitle->setCrop(Crop::CENTER);
	moncraftTitle->setMinFilter(GL_LINEAR);
	moncraftTitle->setMagFilter(GL_LINEAR);
}

void MainMenu::draw() {
	if(parent) {
		setSize(parent->getSize());
	}
	Image::draw();
}
