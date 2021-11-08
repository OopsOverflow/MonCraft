#pragma once

#include <memory>

#include "ui/Box.hpp"
#include "ui/Button.hpp"
#include "ui/Image.hpp"
#include "ui/Text.hpp"

class ParametersMenu : public ui::Image
{
public:
	ParametersMenu();
	void draw() override;

	std::unique_ptr<ui::Button> quitButton;

	std::unique_ptr<ui::Button> keyButton;
	std::unique_ptr<ui::Button> graphicsButton;
	std::unique_ptr<ui::Button> audioButton;
	std::unique_ptr<ui::Button> miscButton;

private:
	std::unique_ptr<ui::Box> mainDisplay;
	std::unique_ptr<ui::Box> topDiv;
	std::unique_ptr<ui::Text> parametersTitle;
	std::unique_ptr<ui::Box> folderDiv;
	std::unique_ptr<ui::Box> menuDiv;

	std::unique_ptr<ui::Box> keyMenu;
	std::unique_ptr<ui::Box> graphicsMenu;
	std::unique_ptr<ui::Box> audioMenu;
	std::unique_ptr<ui::Box> miscMenu;

};
