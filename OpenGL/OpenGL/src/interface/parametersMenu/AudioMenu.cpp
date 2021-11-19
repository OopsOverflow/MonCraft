#include "AudioMenu.hpp"

#include <glm/glm.hpp>

#include "interface/parametersMenu/ParameterSlider.hpp"
#include "gl/ResourceManager.hpp"
#include "ui/Slider.hpp"
#include "ui/style/Type.hpp"

using namespace ui;
using namespace glm;

AudioMenu::AudioMenu()
	: Box()
{
	mainVolume = ParameterSlider::create("Volume principal", Slider::create());;
	musicVolume = ParameterSlider::create("Volume de la musique", Slider::create());;

	setGap(20);
	pack_start(mainVolume.get());
	pack_start(musicVolume.get());
}

void AudioMenu::draw() {
	Box::draw();
}

std::unique_ptr<Box> AudioMenu::create() {
	auto font = ResourceManager::getFont("roboto");
	auto btn = new AudioMenu();
	btn->initialize();
	return std::unique_ptr<Box>(btn);
}
