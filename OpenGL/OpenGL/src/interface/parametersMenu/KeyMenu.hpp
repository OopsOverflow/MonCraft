#pragma once

#include "ui/Box.hpp"

class KeyMenu : public ui::Box {

public:
	static std::unique_ptr<Box> create();

	virtual void draw() override;

protected:
	KeyMenu();

private : 
	std::unique_ptr<ui::Box> forward;
	std::unique_ptr<ui::Box> backward;
	std::unique_ptr<ui::Box> left;
	std::unique_ptr<ui::Box> right;
	std::unique_ptr<ui::Box> jump;
	std::unique_ptr<ui::Box> sneak;
	std::unique_ptr<ui::Box> sprint;
	//std::unique_ptr<ui::Box> break_;
	//std::unique_ptr<ui::Box> place; mathis is tired
	//std::unique_ptr<ui::Box> select;
	std::unique_ptr<ui::Box> changeView;
	std::unique_ptr<ui::Box> debug;

};

