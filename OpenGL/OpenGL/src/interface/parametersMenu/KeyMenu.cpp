#include "KeyMenu.hpp"
#include "ui/Button.hpp"
#include "ParameterButton.hpp"
#include "gl/ResourceManager.hpp"
#include "util/SaveManager.hpp"
#include "SDL2/include/SDL2/SDL_keyboard.h"

using namespace ui;
using namespace glm;

KeyMenu::KeyMenu()
	: Box()
{
	auto config = SaveManager::getInst().getConfig();
	auto font = ResourceManager::getFont("roboto");

	forward = ParameterButton::create("Avancer", Button::createPaneButton(SDL_GetKeyName(config.forward), font));
	backward = ParameterButton::create("Reculer", Button::createPaneButton(SDL_GetKeyName(config.backward), font));
	left = ParameterButton::create("Gauche", Button::createPaneButton(SDL_GetKeyName(config.left), font));
	right = ParameterButton::create("Droite", Button::createPaneButton(SDL_GetKeyName(config.right), font));
	jump = ParameterButton::create("Sauter/Monter", Button::createPaneButton(SDL_GetKeyName(config.jump), font));
	sneak = ParameterButton::create("S'accroupir/Descendre", Button::createPaneButton(SDL_GetKeyName(config.sneak), font));
	sprint = ParameterButton::create("Courir", Button::createPaneButton(SDL_GetKeyName(config.sprint), font));
	changeView = ParameterButton::create("Changer de vue", Button::createPaneButton(SDL_GetKeyName(config.view), font));
	debug = ParameterButton::create("Debug", Button::createPaneButton(SDL_GetKeyName(config.view), font));//TODO

	pack_start(forward.get());
	pack_start(backward.get());
	pack_start(left.get());
	pack_start(right.get());
	pack_start(jump.get());
	pack_start(sneak.get());
	pack_start(sprint.get());
	pack_start(changeView.get());
	pack_start(debug.get());

}

void KeyMenu::draw() {
	Box::draw();
}

std::unique_ptr<Box> KeyMenu::create() {
	auto font = ResourceManager::getFont("roboto");
	auto btn = new KeyMenu();
	btn->initialize();
	return std::unique_ptr<Box>(btn);
}

