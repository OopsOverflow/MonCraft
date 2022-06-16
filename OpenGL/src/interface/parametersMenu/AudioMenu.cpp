#include "AudioMenu.hpp"

#include <glm/glm.hpp>

#include "ui/Slider.hpp"
#include "ui/style/Type.hpp"
#include "interface/widgets/RangeSlider.hpp"
#include "save/ClientConfig.hpp"

using namespace ui;
using namespace glm;

AudioMenu::AudioMenu()
{
	auto config = Config::getClientConfig();

	mainVolume = RangeSlider::create(0, 100);
	musicVolume = RangeSlider::create(0, 100);
	mainVolume->setValue((int)config.mainVolume);
	musicVolume->setValue((int)config.musicVolume);

	addLine("Volume de la musique", musicVolume);
	addLine("Volume principal", mainVolume);

	mainVolume->onRelease([&]{ config.mainVolume = (float)this->mainVolume->getValue(); });
	musicVolume->onRelease([&]{ config.musicVolume = (float)this->musicVolume->getValue(); });
}

AudioMenu::~AudioMenu() {
	auto& config = Config::getClientConfig();

	config.mainVolume = (float)mainVolume->getValue();
	config.musicVolume = (float)musicVolume->getValue();

}

std::unique_ptr<AudioMenu> AudioMenu::create() {
	auto btn = std::unique_ptr<AudioMenu>(new AudioMenu());
	btn->initialize();
	return btn;
}
