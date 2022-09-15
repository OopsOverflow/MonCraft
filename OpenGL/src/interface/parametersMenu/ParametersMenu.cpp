#include "ParametersMenu.hpp"

#include <glm/glm.hpp>

#include "save/ClientConfig.hpp"
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
	add(mainDisplay);

	topDiv->setAnchorX(Anchor::CENTER);
	topDiv->setAnchorY(Anchor::CENTER);
	topDiv->setPadding({ 0, 20 });
	topDiv->setGap(40);
	topDiv->setOrientation(Box::Orientation::HORIZONTAL);
	topDiv->pack_end(Text::create("Parametres", font));

	auto quitButtonImg = Image::create({96, 1179}, {20, 20});
	quitButtonImg->setSize({30, 30});
	quitButton->add(move(quitButtonImg));

	topDiv->pack_end(quitButton);
	mainDisplay->pack_start(topDiv);

	menuDiv->setAnchorX(Anchor::CENTER);
	menuDiv->setAnchorY(Anchor::CENTER);
	menuDiv->setOrientation(Box::Orientation::HORIZONTAL);
	menuDiv->pack_end(graphicsButton);
	menuDiv->pack_end(keyButton);
	#ifndef EMSCRIPTEN
		menuDiv->pack_end(audioButton);
	#endif
	menuDiv->pack_end(miscButton);
	mainDisplay->pack_start(menuDiv);
	folderDiv->pack_start(graphicsMenu);
	mainDisplay->pack_start(folderDiv);

	keyButton->onClick([&] {
		folderDiv->unpackAt(0);
		folderDiv->pack_start(keyMenu);
		this->makeActive();
	});

	graphicsButton->onClick([&] {
		folderDiv->unpackAt(0);
		folderDiv->pack_start(graphicsMenu);
		this->makeActive();
	});

	audioButton->onClick([&] {
		folderDiv->unpackAt(0);
		folderDiv->pack_start(audioMenu);
		this->makeActive();
	});

	miscButton->onClick([&] {
		folderDiv->unpackAt(0);
		folderDiv->pack_start(miscMenu);
		this->makeActive();
	});
}

void ParametersMenu::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Image::draw();
}

std::unique_ptr<ParametersMenu> ParametersMenu::create() {
	auto menu = std::unique_ptr<ParametersMenu>(new ParametersMenu());
	menu->initialize();
	return menu;
}

void ParametersMenu::onKeyReleased(Key k) {
	if(k == Config::getClientConfig().menu){
		quitButton->click();
	}
        
}