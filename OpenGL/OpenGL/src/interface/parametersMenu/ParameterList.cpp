#include "ParameterList.hpp"

#include <glm/glm.hpp>

#include "ui/Text.hpp"
#include "gl/ResourceManager.hpp"
#include "ui/style/Type.hpp"

class Font;

using namespace ui;
using namespace glm;

ParameterList::ParameterList(std::string name, std::shared_ptr<const Font> font, std::vector<std::string> options, size_t index)
	: Box(),
		paramID(index),
		paramNames(options)
{
	setOrientation(ui::Box::Orientation::HORIZONTAL);
	title = ui::Text::create(name, font);
	parameter = ui::Text::create(paramNames.at(index), font);

	leftButton = Button::createImageButton(glm::vec2(14, 1229), glm::vec2(32), "", font);
	leftButton->onclick([&] {
		if (paramID == 0) {
			paramID = this->paramNames.size() - 1;
		}
		else {
			paramID -= 1;
		}
		parameter->setText(this->paramNames.at(paramID));
	});

	rightButton = Button::createImageButton(glm::vec2(49, 1229), glm::vec2(32), "", font);
	rightButton->onclick([&] {
		paramID = (paramID + 1) % this->paramNames.size();
		parameter->setText(this->paramNames.at(paramID));
	});

	pack_end(title.get());
	pack_end(leftButton.get());
	pack_end(parameter.get());
	pack_end(rightButton.get());
}

void ParameterList::draw() {
	Box::draw();
}

std::unique_ptr<Box> ParameterList::create(std::string parameterName, std::vector<std::string> paramNames, size_t value) {
	auto font = ResourceManager::getFont("roboto");
	auto btn = new ParameterList(parameterName, font, paramNames, value);
	btn->initialize();
	return std::unique_ptr<Box>(btn);
}
