#pragma once
#include "ui/ui.hpp"

class ParametersMenu : public ui::Image
{
public:
	ParametersMenu();
	void draw() override;

	std::unique_ptr<ui::Image> quitButton;

	std::unique_ptr<ui::Button> keyButton;
	std::unique_ptr<ui::Button> graphicsButton;
	std::unique_ptr<ui::Button> audioButton;

private:
	std::unique_ptr<ui::Box> mainDisplay;
	std::unique_ptr<ui::Box> topDiv;
	std::unique_ptr<ui::Text> parametersTitle;	
	std::unique_ptr<ui::Box> folderDiv;
	std::unique_ptr<ui::Box> menuDiv;

	std::unique_ptr<ui::Box> keyMenu;
	std::unique_ptr<ui::Box> graphicsMenu;
	std::unique_ptr<ui::Box> audioMenu;
};
