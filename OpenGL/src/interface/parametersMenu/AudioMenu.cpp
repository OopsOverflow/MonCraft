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
	mainVolume->setValue(config.mainVolume);
	musicVolume->setValue(config.musicVolume);

	addLine("Volume principal", mainVolume.get());
	addLine("Volume de la musique", musicVolume.get());
}

AudioMenu::~AudioMenu() {
	auto& config = Config::getClientConfig();

	config.mainVolume = mainVolume->getValue();
	config.musicVolume = musicVolume->getValue();

}

std::unique_ptr<AudioMenu> AudioMenu::create() {
	auto btn = std::unique_ptr<AudioMenu>(new AudioMenu());
	btn->initialize();
	return btn;
}
