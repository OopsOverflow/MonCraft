#pragma once

#include <memory>

#include "ui/Box.hpp"
#include "ui/Button.hpp"
#include "ui/Image.hpp"
#include "ui/Text.hpp"

#include "interface/parametersMenu/AudioMenu.hpp"
#include "interface/parametersMenu/KeyMenu.hpp"
#include "interface/parametersMenu/MiscMenu.hpp"
#include "interface/parametersMenu/GraphicsMenu.hpp"

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

	std::unique_ptr<GraphicsMenu> graphicsMenu;

private:
	std::unique_ptr<ui::Box> mainDisplay;
	std::unique_ptr<ui::Box> topDiv;
	std::unique_ptr<ui::Box> folderDiv;
	std::unique_ptr<ui::Box> menuDiv;

	std::unique_ptr<KeyMenu> keyMenu;
	std::unique_ptr<AudioMenu> audioMenu;
	std::unique_ptr<MiscMenu> miscMenu;
};
