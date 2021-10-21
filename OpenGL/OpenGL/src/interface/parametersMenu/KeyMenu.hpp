#pragma once

#include "ui/Box.hpp"

class KeyMenu : public ui::Box {

public:
	static std::unique_ptr<Box> create();

	virtual void draw() override;

protected:
	KeyMenu();

};

