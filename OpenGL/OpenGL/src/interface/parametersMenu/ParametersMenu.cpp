#include "ParametersMenu.hpp"

#include <glm/glm.hpp>

#include "interface/parametersMenu/AudioMenu.hpp"
#include "interface/parametersMenu/KeyMenu.hpp"
#include "interface/parametersMenu/MiscMenu.hpp"
#include "interface/parametersMenu/GraphicsMenu.hpp"
#include "gl/ResourceManager.hpp"
#include "ui/Component.hpp"
#include "ui/style/Type.hpp"

using namespace ui;

ParametersMenu::ParametersMenu() : Image({ 1, 1179 }, { 46, 46 })
{
	setCrop(Crop::BEGIN);
	setPadding(glm::vec2(10));
	auto font = ResourceManager::getFont("roboto");

	parametersTitle = Text::create("Parametres", font);
	mainDisplay = Box::create();
	topDiv = Box::create();
	topDiv->setOrientation(Box::Orientation::HORIZONTAL);

	folderDiv = Box::create();
	menuDiv = Box::create();
	menuDiv->setOrientation(Box::Orientation::HORIZONTAL);

	quitButton = Button::create();
	quitButton->add(Image::create(glm::vec2(1923, 1), glm::vec2(659, 657)));
	keyButton = Button::createPaneButton("Controles", font);
	graphicsButton = Button::createPaneButton("Graphismes", font);
	audioButton = Button::createPaneButton("Audio", font);
	miscButton = Button::createPaneButton("Divers", font);

	keyMenu = KeyMenu::create();
	graphicsMenu = GraphicsMenu::create();
	audioMenu = AudioMenu::create();
	miscMenu = MiscMenu::create();

	add(mainDisplay.get());
	mainDisplay->setAnchorX(Anchor::CENTER);
	mainDisplay->setAnchorY(Anchor::CENTER);

	mainDisplay->pack_start(topDiv.get());
	topDiv->setAnchorX(Anchor::CENTER);
	topDiv->setAnchorY(Anchor::CENTER);

	mainDisplay->pack_start(folderDiv.get());

	topDiv->pack_end(quitButton.get());
	quitButton->setAnchorX(Anchor::CENTER);
	quitButton->setAnchorY(Anchor::CENTER);
	quitButton->setSize(glm::vec2(50, 50));
//TODO quitbutton hover

	topDiv->pack_end(parametersTitle.get());

	folderDiv->pack_start(menuDiv.get());
	menuDiv->setAnchorX(Anchor::CENTER);
	menuDiv->setAnchorY(Anchor::CENTER);

	folderDiv->pack_start(keyMenu.get());

	menuDiv->pack_end(keyButton.get());
	keyButton->onclick([&] {
		folderDiv->unpack(keyMenu.get());
		folderDiv->unpack(graphicsMenu.get());
		folderDiv->unpack(miscMenu.get());
		folderDiv->unpack(audioMenu.get());
		folderDiv->pack_start(keyMenu.get());
		});

	menuDiv->pack_end(graphicsButton.get());
	graphicsButton->onclick([&] {
		folderDiv->unpack(keyMenu.get());
		folderDiv->unpack(graphicsMenu.get());
		folderDiv->unpack(miscMenu.get());
		folderDiv->unpack(audioMenu.get());
		folderDiv->pack_start(graphicsMenu.get());
		});

	menuDiv->pack_end(audioButton.get());
	audioButton->onclick([&] {
		folderDiv->unpack(keyMenu.get());
		folderDiv->unpack(graphicsMenu.get());
		folderDiv->unpack(miscMenu.get());
		folderDiv->unpack(audioMenu.get());
		folderDiv->pack_start(audioMenu.get());
		});

	menuDiv->pack_end(miscButton.get());
	miscButton->onclick([&] {
		folderDiv->unpack(keyMenu.get());
		folderDiv->unpack(graphicsMenu.get());
		folderDiv->unpack(miscMenu.get());
		folderDiv->unpack(audioMenu.get());
		folderDiv->pack_start(miscMenu.get());
		});

	//quitButton->onclick([&] {
	//	SaveManager::getInst().saveConfig(this->config);

	//	})
}

void ParametersMenu::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Image::draw();
}
