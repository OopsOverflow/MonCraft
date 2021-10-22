#include "ParameterList.hpp"
#include "ui/Text.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;
using namespace glm;

ParameterList::ParameterList(std::string parameterName, std::shared_ptr<const Font> font, std::vector<std::string> paramNames, size_t index)
	:Box()
{
	setOrientation(ui::Box::Orientation::HORIZONTAL);
	title = ui::Text::create(parameterName, font);
	parameter = ui::Text::create(paramNames.at(paramID), font);
	this->paramNames = paramNames;
	this->paramID = index;

	leftButton = Button::createImageButton(glm::vec2(14,1229), glm::vec2(32),"", font);
	leftButton->onclick([&] {
		if (paramID == 0) {
			paramID = this->paramNames.size()-1;
		}
		else {
			paramID -= 1;
		}
		parameter->setText(this->paramNames.at(paramID));
		});

	rightButton = Button::createImageButton(glm::vec2(49,1229), glm::vec2(32),"", font);
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
