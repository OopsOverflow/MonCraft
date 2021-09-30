#pragma once
#include "ui/ui.hpp"

class MainMenu : public ui::Component
{
public:
	MainMenu();
	void draw() override;

	std::unique_ptr<ui::Button> playButton;

private:
	std::unique_ptr<ui::Pane> titlePane;
	std::unique_ptr<ui::Text> moncraftTitle;

};
