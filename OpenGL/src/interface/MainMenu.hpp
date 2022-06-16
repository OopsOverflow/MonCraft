#pragma once

#include <memory>

#include "ui/Button.hpp"
#include "ui/Image.hpp"
class MainMenu : public ui::Component
{
protected:
	MainMenu();
	
public:
	static std::unique_ptr<MainMenu> create();
	void draw() override;

	std::shared_ptr<ui::Button> singleplayerButton;
	std::shared_ptr<ui::Button> multiplayerButton;
	std::shared_ptr<ui::Button> parameterButton;
	std::shared_ptr<ui::Button> quitButton;

private:
	std::shared_ptr<ui::Image> background;
};
