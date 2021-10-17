#include "MainMenu.hpp"
#include "gl/ResourceManager.hpp"
#include "MainMenuButton.hpp"

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

	buttonPannel->pack_end(singleplayerButton.get());
	buttonPannel->pack_end(multiplayerButton.get());
	#ifndef EMSCRIPTEN
		buttonPannel->pack_end(parameterButton.get());
		buttonPannel->pack_end(quitButton.get());
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
