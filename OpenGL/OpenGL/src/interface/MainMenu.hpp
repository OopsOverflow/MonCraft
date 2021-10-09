#pragma once
#include "ui/ui.hpp"

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
