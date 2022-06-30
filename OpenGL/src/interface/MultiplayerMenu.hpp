#pragma once
#include "ui/ui.hpp"

class MultiplayerMenu : public ui::Image
{
public:
	void draw() override;
	static std::unique_ptr<MultiplayerMenu> create();

	std::shared_ptr<ui::Input> serverAddrInput;
	std::shared_ptr<ui::Input> portInput;


private:
	MultiplayerMenu();
	std::shared_ptr<ui::Image> serverAddrImg;
	std::shared_ptr<ui::Image> portImg;

};
