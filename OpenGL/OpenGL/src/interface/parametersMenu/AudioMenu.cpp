#include "AudioMenu.hpp"

#include <glm/glm.hpp>

#include "ui/Slider.hpp"
#include "ui/style/Type.hpp"
#include "interface/widgets/RangeSlider.hpp"

using namespace ui;
using namespace glm;

AudioMenu::AudioMenu()
{
	mainVolume = RangeSlider::create(0, 100);
	musicVolume = RangeSlider::create(0, 100);

	addLine("Volume principal", mainVolume.get());
	addLine("Volume de la musique", musicVolume.get());
}

std::unique_ptr<AudioMenu> AudioMenu::create() {
	auto btn = std::unique_ptr<AudioMenu>(new AudioMenu());
	btn->initialize();
	return btn;
}
