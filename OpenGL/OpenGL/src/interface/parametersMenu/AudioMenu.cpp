#include "AudioMenu.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;
using namespace glm;

AudioMenu::AudioMenu()
	: Box()
{}

void AudioMenu::draw() {
	Box::draw();
}

std::unique_ptr<Box> AudioMenu::create() {
	auto font = ResourceManager::getFont("roboto");
	auto btn = new AudioMenu();
	btn->initialize();
	return std::unique_ptr<Box>(btn);
}
