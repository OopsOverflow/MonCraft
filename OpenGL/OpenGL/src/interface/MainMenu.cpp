#include "MainMenu.hpp"
#include "gl/ResourceManager.hpp"

MainMenu::MainMenu() : Image({0, 0}, {1920, 1080})
{

	setCrop(Crop::CENTER);
	setMinFilter(GL_LINEAR);
	setMagFilter(GL_LINEAR);
	setPadding(glm::ivec2(15, 15));
	auto font = ResourceManager::getFont("roboto");

	moncraftTitle = std::make_unique<ui::Image>(glm::ivec2(0, 1083), glm::ivec2(429, 92));
	buttonPannel = std::make_unique<ui::Box>();

	singlePlayerButton = std::make_unique<ui::Button>("Solo", font);
	multiplayerButton = std::make_unique<ui::Button>("Multijoueur", font);
	parameterButton = std::make_unique<ui::Button>("Parametres", font);
	quitButton = std::make_unique<ui::Button>("Quitter", font);
	
	add(moncraftTitle.get());
	add(buttonPannel.get());

	buttonPannel->setAnchorX(ui::Anchor::CENTER);
	buttonPannel->setAnchorY(ui::Anchor::CENTER);

	buttonPannel->pack(singlePlayerButton.get());
	buttonPannel->pack(multiplayerButton.get());
	buttonPannel->pack(parameterButton.get());
	buttonPannel->pack(quitButton.get());

	moncraftTitle->setAnchorX(ui::Anchor::CENTER);
	moncraftTitle->setAnchorY(ui::Anchor::END);

	moncraftTitle->setSize({ 1.5*429,1.5*92 });
	moncraftTitle->setCrop(Crop::CENTER);
	moncraftTitle->setMinFilter(GL_LINEAR);
	moncraftTitle->setMagFilter(GL_LINEAR);


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
