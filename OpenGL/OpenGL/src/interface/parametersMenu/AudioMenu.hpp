#pragma once

#include <memory>

#include "ui/Box.hpp"

class AudioMenu : public ui::Box {

public:
	static std::unique_ptr<Box> create();

	virtual void draw() override;

protected:
	AudioMenu();

private:
	std::unique_ptr<ui::Box> mainVolume;
	std::unique_ptr<ui::Box> musicVolume;
};
