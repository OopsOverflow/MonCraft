#pragma once

#include <memory>

#include "ui/Box.hpp"

class GraphicsMenu : public ui::Box {

public:
	static std::unique_ptr<Box> create();

	virtual void draw() override;

protected:
	GraphicsMenu();

private: 
	std::unique_ptr<ui::Box> fullscreen;
	std::unique_ptr<ui::Box> fov;
	std::unique_ptr<ui::Box> shadows;
	std::unique_ptr<ui::Box> renderDistanceH;
	std::unique_ptr<ui::Box> renderDistanceV;
	std::unique_ptr<ui::Box> vSync;

};
