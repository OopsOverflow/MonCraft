#pragma once
#include "ui/ui.hpp"

class Overlay : public ui::Component
{
public:
	Overlay();
	void draw() override;
	std::unique_ptr <ui::Button> btn_vsync;
	std::unique_ptr <ui::Button> btn_fullscreen;
	std::unique_ptr <ui::Button> btn_ping;
	std::unique_ptr <ui::Button> btn_block;

private:

};
