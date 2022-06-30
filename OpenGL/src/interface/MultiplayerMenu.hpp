#pragma once
#include "ui/ui.hpp"

class MultiplayerMenu : public ui::Image
{
public:
	MultiplayerMenu();
	void draw() override;

	std::unique_ptr<ui::Input> serverAddrInput;
	std::unique_ptr<ui::Input> portInput;


private:
	std::unique_ptr<ui::Image> serverAddrImg;
	std::unique_ptr<ui::Image> portImg;

};
