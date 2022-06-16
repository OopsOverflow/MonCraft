#pragma once

#include <memory>

#include "ui/Box.hpp"
#include "interface/widgets/ParamList.hpp"
#include "interface/widgets/RangeSlider.hpp"

class AudioMenu : public ParamList {

public:
	static std::unique_ptr<AudioMenu> create();
	~AudioMenu();

protected:
	AudioMenu();

private:
	std::shared_ptr<RangeSlider> mainVolume;
	std::shared_ptr<RangeSlider> musicVolume;
};
