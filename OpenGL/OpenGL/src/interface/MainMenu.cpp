#include "MainMenu.hpp"
#include "gl/ResourceManager.hpp"

MainMenu::MainMenu(Component* parent) : 
	Component(parent)
{
	auto font = ResourceManager::getFont("roboto");
	titlePane = std::make_unique<ui::Pane>(this);
	moncraftTitle = std::make_unique<ui::Text>(titlePane.get(),"MONCRAFT",font);
	playButton = std::make_unique<ui::Button>(this,"PLAY",font);


	titlePane->setAnchorX(ui::Anchor::CENTER);
	titlePane->setAnchorY(ui::Anchor::END);

}

void MainMenu::draw() {
	Component::draw();
	std::cout<< parent->getSize()<<std::endl;
}