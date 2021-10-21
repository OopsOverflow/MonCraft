#pragma once

#include "ui/Box.hpp"

class AudioMenu : public ui::Box {

public:
	static std::unique_ptr<Box> create();

	virtual void draw() override;

protected:
	AudioMenu();

};
