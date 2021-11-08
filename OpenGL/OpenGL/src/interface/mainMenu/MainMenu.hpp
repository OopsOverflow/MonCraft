#pragma once

#include <memory>

#include "ui/Box.hpp"
#include "ui/Button.hpp"
#include "ui/Image.hpp"

class MainMenu : public ui::Image
{
public:
	MainMenu();
	void draw() override;

	std::unique_ptr<ui::Button> singleplayerButton;
	std::unique_ptr<ui::Button> multiplayerButton;
	std::unique_ptr<ui::Button> parameterButton;
	std::unique_ptr<ui::Button> quitButton;


private:
	std::unique_ptr<ui::Image> moncraftTitle;
	std::unique_ptr<ui::Box> buttonPannel;

};
