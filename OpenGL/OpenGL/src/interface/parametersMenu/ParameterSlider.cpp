#include "ParameterSlider.hpp"

#include <glm/glm.hpp>
#include <utility>

#include "gl/ResourceManager.hpp"
#include "ui/Text.hpp"
#include "ui/style/Type.hpp"

class Font;

using namespace ui;
using namespace glm;

ParameterSlider::ParameterSlider(std::string parameterName, std::shared_ptr<const Font> font, std::unique_ptr<ui::Slider> inputComponent)
	:Box()
{
	setOrientation(ui::Box::Orientation::HORIZONTAL);
	title = ui::Text::create(parameterName, font);
	slider = std::move(inputComponent);
	sliderValue = ui::Text::create(std::to_string(slider->getValue()), font);

	setGap(20);
	pack_end(title.get());
	pack_end(slider.get());
	pack_end(sliderValue.get());
}

void ParameterSlider::draw() {
	Box::draw();
}

std::unique_ptr<Box> ParameterSlider::create(std::string parameterName, std::unique_ptr<ui::Slider> inputComponent) {
	auto font = ResourceManager::getFont("roboto");
	auto btn = new ParameterSlider(parameterName, font, std::move(inputComponent));
	btn->initialize();
	return std::unique_ptr<Box>(btn);
}
