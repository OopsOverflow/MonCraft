#include "ParametersMenu.hpp"
#include "gl/ResourceManager.hpp"

ParametersMenu::ParametersMenu() : Image({ 0, 1178 }, { 132, 116 })
{
	auto font = ResourceManager::getFont("roboto");

	parametersTitle = std::make_unique<ui::Text>("Parametres", font);
	folders = std::make_unique<ui::Box>();
	display = std::make_unique<ui::Box>();

	keyDisplay = std::make_unique<ui::Box>();
	graphicDisplay = std::make_unique<ui::Box>();
	audioDisplay = std::make_unique<ui::Box>();

	keyMenu = std::make_unique<ui::Button>("Controles", font);
	graphicMenu = std::make_unique<ui::Button>("Graphismes", font);
	audioMenu = std::make_unique<ui::Button>("Audio", font);
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
