#include "ParametersMenu.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;

ParametersMenu::ParametersMenu() : Image({ 0, 1178 }, { 132, 116 })
{
	auto font = ResourceManager::getFont("roboto");

	parametersTitle = Text::create("Parametres", font);
	folders = Box::create();
	display = Box::create();

	keyDisplay = Box::create();
	graphicDisplay = Box::create();
	audioDisplay = Box::create();

	keyMenu = Button::createPaneButton("Controles", font);
	graphicMenu = Button::createPaneButton("Graphismes", font);
	audioMenu = Button::createPaneButton("Audio", font);
	add(display.get());

	display->add(parametersTitle.get());
	display->add(folders.get());

	folders->add(keyMenu.get());
	folders->add(graphicMenu.get());
	folders->add(audioMenu.get());

	setSize(getSize() * 2);
}

void ParametersMenu::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Image::draw();
}
