#pragma once

#include <memory>

#include "ui/Button.hpp"
#include "ui/Image.hpp"

#include <iostream>
class MainMenu : public ui::Component
{
protected:
	MainMenu();
	
public:
	static std::unique_ptr<MainMenu> create();
	void draw() override;
	~MainMenu() {std::cout<<"ui"<<std::endl;}

	std::unique_ptr<ui::Button> singleplayerButton;
	std::unique_ptr<ui::Button> multiplayerButton;
	std::unique_ptr<ui::Button> parameterButton;
	std::unique_ptr<ui::Button> quitButton;

private:
	std::unique_ptr<ui::Image> background;
};
