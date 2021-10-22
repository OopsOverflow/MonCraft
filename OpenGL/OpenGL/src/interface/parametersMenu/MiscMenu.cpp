#include "MiscMenu.hpp"
#include "ParameterButton.hpp"
#include "ParameterSlider.hpp"
#include "ParameterList.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;
using namespace glm;

MiscMenu::MiscMenu()
	: Box()
{
	auto font = ResourceManager::getFont("roboto");

	auto button = Button::create();
	glm::vec2 imageOffset;
	glm::vec2 imageSize;
	if (1) {
		imageOffset = glm::vec2(1923, 1);
		imageSize = glm::vec2(659, 657);
	}
	else {
		imageOffset = glm::vec2(2159, 0);
		imageSize = glm::vec2(1, 1);
	}

	auto buttonImage = Image::create(imageOffset, imageSize);
	buttonImage->setSize(glm::vec2(50));
	button->add(std::move(buttonImage));
	enableMobileVersionButton = ParameterButton::create("Version Mobile", std::move(button));;

	pack_start(enableMobileVersionButton.get());
}

void MiscMenu::draw() {
	Box::draw();
}

std::unique_ptr<Box> MiscMenu::create() {

	auto btn = new MiscMenu();
	btn->initialize();
	return std::unique_ptr<Box>(btn);
}
