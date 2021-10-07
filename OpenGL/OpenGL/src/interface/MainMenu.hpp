#pragma once
#include "ui/ui.hpp"

class MainMenu : public ui::Image
{
public:
	MainMenu();
	void draw() override;

	std::unique_ptr<ui::Button> playButton;

private:
	std::unique_ptr<ui::Image> MoncraftTitle;

};
