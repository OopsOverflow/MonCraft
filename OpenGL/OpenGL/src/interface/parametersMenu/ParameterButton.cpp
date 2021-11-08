#include "ParameterButton.hpp"

#include <glm/glm.hpp>

#include "gl/ResourceManager.hpp"
#include "ui/Text.hpp"
#include "ui/style/Type.hpp"

class Font;

using namespace ui;
using namespace glm;

ParameterButton::ParameterButton(std::string parameterName, std::shared_ptr<const Font> font, std::unique_ptr<ui::Button> inputComponent)
	:Box()
{
	setOrientation(ui::Box::Orientation::HORIZONTAL);
	title = ui::Text::create(parameterName, font);

	button = std::move(inputComponent);

	pack_end(title.get());
	pack_end(button.get());
}

void ParameterButton::draw() {
	Box::draw();
}

std::unique_ptr<Box> ParameterButton::create(std::string parameterName, std::unique_ptr<ui::Button> inputComponent) {
	auto font = ResourceManager::getFont("roboto");
	auto btn = new ParameterButton(parameterName, font, std::move(inputComponent));
	btn->initialize();
	return std::unique_ptr<Box>(btn);
}
