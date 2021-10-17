#include "ParametersMenu.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;

ParametersMenu::ParametersMenu() : Image({ 1, 1179 }, { 46, 46 })
{
	setCrop(Crop::BEGIN);
	setPadding(glm::vec2(10));
	auto font = ResourceManager::getFont("roboto");

	parametersTitle = Text::create("Parametres", font);
	folders = Box::create();
	display = Box::create();

	keyDisplay = Box::create();
	graphicDisplay = Box::create();
	audioDisplay = Box::create();

	quitButton = Image::create(glm::vec2(1923, 1), glm::vec2(659, 657));
	keyMenu = Button::createPaneButton("Controles", font);
	graphicMenu = Button::createPaneButton("Graphismes", font);
	audioMenu = Button::createPaneButton("Audio", font);

	add(display.get());
	display->setSize(getAbsoluteOrigin());

	quitButton->setAnchorX(Anchor::BEGIN);
	quitButton->setAnchorY(Anchor::END);
	quitButton->setSize(glm::vec2(50, 50));
	display->add(quitButton.get());
	display->add(parametersTitle.get());
	display->add(folders.get());

	folders->add(keyMenu.get());
	folders->add(graphicMenu.get());
	folders->add(audioMenu.get());
}

void ParametersMenu::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Image::draw();
}
