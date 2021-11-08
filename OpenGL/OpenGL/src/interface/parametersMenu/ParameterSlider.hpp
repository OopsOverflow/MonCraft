#pragma once

#include <memory>
#include <string>

#include "ui/Box.hpp"
#include "ui/Slider.hpp"
#include "ui/Text.hpp"

class Font;

class ParameterSlider : public ui::Box {

public:
	static std::unique_ptr<Box> create(std::string parameterName, std::unique_ptr<ui::Slider> inputComponent);

	virtual void draw() override;

	std::unique_ptr<ui::Slider> slider;

protected:
	ParameterSlider(std::string parameterName, std::shared_ptr<const Font> font, std::unique_ptr<ui::Slider> inputComponent);
private:
	std::unique_ptr<ui::Text> title;
	std::unique_ptr<ui::Text> sliderValue;
};
