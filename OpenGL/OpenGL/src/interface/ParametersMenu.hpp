#pragma once
#include "ui/ui.hpp"

class ParametersMenu : public ui::Image
{
public:
	ParametersMenu();
	void draw() override;

	std::unique_ptr<ui::Image> quitButton;
	std::unique_ptr<ui::Button> keyMenu;
	std::unique_ptr<ui::Button> graphicMenu;
	std::unique_ptr<ui::Button> audioMenu;

private:
	std::unique_ptr<ui::Text> parametersTitle;
	std::unique_ptr<ui::Box> folders;
	std::unique_ptr<ui::Box> display;
		
	std::unique_ptr<ui::Box> keyDisplay;
	std::unique_ptr<ui::Box> graphicDisplay;
	std::unique_ptr<ui::Box> audioDisplay;

};
