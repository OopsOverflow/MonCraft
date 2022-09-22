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
	static std::unique_ptr<ParametersMenu> create();
	void draw() override;

	std::shared_ptr<ui::Button> quitButton;

	std::shared_ptr<ui::Button> keyButton;
	std::shared_ptr<ui::Button> graphicsButton;
	std::shared_ptr<ui::Button> audioButton;
	std::shared_ptr<ui::Button> miscButton;

	std::shared_ptr<GraphicsMenu> graphicsMenu;
	std::shared_ptr<AudioMenu> audioMenu;

protected:
	virtual void onKeyReleased(Key k) override;

private:
	ParametersMenu();
	std::shared_ptr<ui::Box> mainDisplay;
	std::shared_ptr<ui::Box> topDiv;
	std::shared_ptr<ui::Box> folderDiv;
	std::shared_ptr<ui::Box> menuDiv;

	std::shared_ptr<KeyMenu> keyMenu;
	std::shared_ptr<MiscMenu> miscMenu;
};
