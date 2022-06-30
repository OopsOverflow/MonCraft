#pragma once

#include <memory>

#include "ui/Box.hpp"
#include "interface/widgets/ParamList.hpp"
#include "interface/widgets/RangeSlider.hpp"

class AudioMenu : public ParamList {

public:
	static std::unique_ptr<AudioMenu> create();

protected:
	AudioMenu();

private:
	std::unique_ptr<RangeSlider> mainVolume;
	std::unique_ptr<RangeSlider> musicVolume;
};
