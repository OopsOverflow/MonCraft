#include "GraphicsMenu.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "interface/parametersMenu/ParameterButton.hpp"
#include "interface/parametersMenu/ParameterSlider.hpp"
#include "interface/parametersMenu/ParameterList.hpp"
#include "gl/ResourceManager.hpp"
#include "ui/Button.hpp"
#include "ui/Slider.hpp"
#include "ui/style/Type.hpp"

using namespace ui;
using namespace glm;

GraphicsMenu::GraphicsMenu()
	: Box()
{
	auto font = ResourceManager::getFont("roboto");
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

	fullscreen = ParameterButton::create("Plein Ecran", Button::createImageButton(imageOffset, imageSize,"" , font));
	fov = ParameterSlider::create("Fov", Slider::create());

	std::vector<std::string> shadowType = { "None", "Low", "High" };
	shadows = ParameterList::create("Ombres", shadowType, 2);
	renderDistanceH = ParameterSlider::create("Distance horizontale", Slider::create());;
	renderDistanceV = ParameterSlider::create("Distance verticale", Slider::create());;
	vSync = ParameterButton::create("VSync", Button::createImageButton(imageOffset, imageSize, "", font));;

	pack_start(fullscreen.get());
	pack_start(fov.get());
	pack_start(shadows.get());
	pack_start(renderDistanceH.get());
	pack_start(renderDistanceV.get());
	pack_start(vSync.get());
}

void GraphicsMenu::draw() {
	Box::draw();
}

std::unique_ptr<Box> GraphicsMenu::create() {
	auto font = ResourceManager::getFont("roboto");
	auto btn = new GraphicsMenu();
	btn->initialize();
	return std::unique_ptr<Box>(btn);
}
