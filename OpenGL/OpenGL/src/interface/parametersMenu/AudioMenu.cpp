#include "AudioMenu.hpp"
#include "ParameterButton.hpp"
#include "ParameterSlider.hpp"
#include "ParameterList.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;
using namespace glm;

AudioMenu::AudioMenu()
	: Box()
{
	mainVolume = ParameterSlider::create("Volume principal", Slider::create());;
	musicVolume = ParameterSlider::create("Volume de la musique", Slider::create());;

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
