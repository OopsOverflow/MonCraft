#pragma once

#include <memory>

#include "ui/Box.hpp"

class MiscMenu : public ui::Box {

public:
	static std::unique_ptr<Box> create();

	virtual void draw() override;

protected:
	MiscMenu();

private:
	std::unique_ptr<ui::Box> enableMobileVersionButton;
};
