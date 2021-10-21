#pragma once

#include "ui/Box.hpp"

class GraphicsMenu : public ui::Box {

public:
	static std::unique_ptr<Box> create();

	virtual void draw() override;

protected:
	GraphicsMenu();

};
