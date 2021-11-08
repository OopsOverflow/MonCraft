#pragma once

#include <memory>

#include "ui/Button.hpp"
#include "ui/Component.hpp"

class Overlay : public ui::Component
{
public:
	Overlay();
	void draw() override;

	std::unique_ptr <ui::Button> btn_block;

private:

};
