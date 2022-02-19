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
	std::unique_ptr<ui::Box> folderDiv;
	std::unique_ptr<ui::Box> menuDiv;

	std::unique_ptr<ui::Component> keyMenu;
	std::unique_ptr<ui::Component> graphicsMenu;
	std::unique_ptr<ui::Component> audioMenu;
	std::unique_ptr<ui::Component> miscMenu;
};
