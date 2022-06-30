#pragma once

#include <memory>

#include "ui/Button.hpp"
#include "ui/Component.hpp"
#include "ui/Pane.hpp"
#include "ui/Text.hpp"

class GameMenu : public ui::Component
{
public:
	GameMenu();
	void draw() override;

	std::unique_ptr<ui::Button> playButton;
	std::unique_ptr<ui::Button> quitButton;

private:
	std::unique_ptr<ui::Pane> titlePane;
	std::unique_ptr<ui::Text> pauseTitle;

};