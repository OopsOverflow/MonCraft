#include "ParametersMenu.hpp"

#include <glm/glm.hpp>

#include "interface/parametersMenu/AudioMenu.hpp"
#include "interface/parametersMenu/KeyMenu.hpp"
#include "interface/parametersMenu/MiscMenu.hpp"
#include "interface/parametersMenu/GraphicsMenu.hpp"
#include "interface/widgets/MonCraftButton.hpp"
#include "gl/ResourceManager.hpp"
#include "ui/Component.hpp"
#include "ui/style/Type.hpp"

using namespace ui;

ParametersMenu::ParametersMenu()
	: Image({ 1, 1179 }, { 46, 46 })
{
	setCrop(Crop::BEGIN);
	setPadding(glm::vec2(10));
	auto font = ResourceManager::getFont("roboto");

	mainDisplay = Box::create();
	topDiv = Box::create();
	folderDiv = Box::create();
	menuDiv = Box::create();
	quitButton = Button::create();

	keyButton = MonCraftButton::create("Controles");
	graphicsButton = MonCraftButton::create("Graphismes");
	audioButton = MonCraftButton::create("Audio");
	miscButton = MonCraftButton::create("Divers");

	keyMenu = KeyMenu::create();
	graphicsMenu = GraphicsMenu::create();
	audioMenu = AudioMenu::create();
	miscMenu = MiscMenu::create();

	mainDisplay->setAnchorX(Anchor::CENTER);
	mainDisplay->setAnchorY(Anchor::END);
	add(mainDisplay.get());

	topDiv->setAnchorX(Anchor::CENTER);
	topDiv->setAnchorY(Anchor::CENTER);
	topDiv->setPadding({ 0, 20 });
	topDiv->setGap(40);
	topDiv->setOrientation(Box::Orientation::HORIZONTAL);
	topDiv->pack_end(Text::create("Parametres", font));

	auto quitButtonImg = Image::create({96, 1179}, {20, 20});
	quitButtonImg->setSize({30, 30});
	quitButton->add(move(quitButtonImg));

	topDiv->pack_end(quitButton.get());
	mainDisplay->pack_start(topDiv.get());

	menuDiv->setAnchorX(Anchor::CENTER);
	menuDiv->setAnchorY(Anchor::CENTER);
	menuDiv->setOrientation(Box::Orientation::HORIZONTAL);
	menuDiv->pack_end(graphicsButton.get());
	menuDiv->pack_end(keyButton.get());
	menuDiv->pack_end(audioButton.get());
	menuDiv->pack_end(miscButton.get());
	mainDisplay->pack_start(menuDiv.get());
	folderDiv->pack_start(graphicsMenu.get());
	mainDisplay->pack_start(folderDiv.get());

	keyButton->onclick([&] {
		folderDiv->unpackAt(0);
		folderDiv->pack_start(keyMenu.get());
	});

	graphicsButton->onclick([&] {
		folderDiv->unpackAt(0);
		folderDiv->pack_start(graphicsMenu.get());
	});

	audioButton->onclick([&] {
		folderDiv->unpackAt(0);
		folderDiv->pack_start(audioMenu.get());
	});

	miscButton->onclick([&] {
		folderDiv->unpackAt(0);
		folderDiv->pack_start(miscMenu.get());
	});
}

void ParametersMenu::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Image::draw();
}
