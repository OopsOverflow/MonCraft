#include "MainMenu.hpp"
#include "gl/ResourceManager.hpp"

MainMenu::MainMenu() : Image({0, 0}, {1920, 1080})
{

	setCrop(Crop::CENTER);
	auto font = ResourceManager::getFont("roboto");

	moncraftTitle = std::make_unique<ui::Image>(glm::ivec2(0, 1081), glm::ivec2(438, 100));
	buttonPannel = std::make_unique<ui::Box>();

	singlePlayerButton = ui::Button::makePaneButton("Solo", font);
	multiplayerButton = ui::Button::makePaneButton("Multijoueur", font);
	parameterButton = ui::Button::makePaneButton("Parametres", font);
	quitButton = ui::Button::makePaneButton("Quitter", font);

	add(moncraftTitle.get());
	add(buttonPannel.get());

	buttonPannel->setAnchorX(ui::Anchor::CENTER);
	buttonPannel->setAnchorY(ui::Anchor::CENTER);
	buttonPannel->setGap(10);

	buttonPannel->pack_end(singlePlayerButton.get());
	buttonPannel->pack_end(multiplayerButton.get());
	buttonPannel->pack_end(parameterButton.get());
	buttonPannel->pack_end(quitButton.get());

	moncraftTitle->setAnchorX(ui::Anchor::CENTER);
	moncraftTitle->setAnchorY(ui::Anchor::END);

	moncraftTitle->setSize({ 1.5*438,1.5*100 });
	moncraftTitle->setCrop(Crop::CENTER);

	singlePlayerButton->setAnchorX(ui::Anchor::CENTER);
	multiplayerButton->setAnchorX(ui::Anchor::CENTER);
	parameterButton->setAnchorX(ui::Anchor::CENTER);
	quitButton->setAnchorX(ui::Anchor::CENTER);

}

void MainMenu::draw() {
	if(parent) {
		setSize(parent->getSize());
	}
	Image::draw();
}
