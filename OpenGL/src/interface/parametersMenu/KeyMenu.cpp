#include "KeyMenu.hpp"

#include <utility>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "interface/widgets/KeySelector.hpp"
#include "gl/ResourceManager.hpp"
#include "save/ClientConfig.hpp"
#include "ui/Button.hpp"
#include "ui/style/Type.hpp"

using namespace ui;
using namespace glm;

KeyMenu::KeyMenu()
{
	auto& config = Config::getClientConfig();
	auto font = ResourceManager::getFont("roboto");

	forward = KeySelector::create(config.forward);
	backward = KeySelector::create(config.backward);
	left = KeySelector::create(config.left);
	right = KeySelector::create(config.right);
	jump = KeySelector::create(config.jump);
	sneak = KeySelector::create(config.sneak);
	sprint = KeySelector::create(config.sprint);
	dab = KeySelector::create(config.dab);
	changeView = KeySelector::create(config.view);
	// debug = KeySelector::create(config.debug); // TODO
	
	// addLine("Menu débogage", debug.get());
	addLine("Changer de vue", changeView);
	addLine("Dabber", dab);
	addLine("Courir", sprint);
	addLine("S'accroupir/Descendre", sneak);
	addLine("Sauter/Monter", jump);
	addLine("Droite", right);
	addLine("Gauche", left);
	addLine("Reculer", backward);
	addLine("Avancer", forward);

	forward->onRelease([&]{ config.forward = forward->getKey().asKeycode(); });
	backward->onRelease([&]{ config.backward = backward->getKey().asKeycode(); });
	left->onRelease([&]{ config.left = left->getKey().asKeycode(); });
	right->onRelease([&]{ config.right = right->getKey().asKeycode(); });
	jump->onRelease([&]{ config.jump = jump->getKey().asKeycode(); });
	sneak->onRelease([&]{ config.sneak = sneak->getKey().asKeycode(); });
	sprint->onRelease([&]{ config.sprint = sprint->getKey().asKeycode(); });
	dab->onRelease([&]{ config.dab = dab->getKey().asKeycode(); });
	changeView->onRelease([&]{ config.view = changeView->getKey().asKeycode(); });
}

KeyMenu::~KeyMenu() {
	auto& config = Config::getClientConfig();
	config.forward = forward->getKey().asKeycode();
	config.backward = backward->getKey().asKeycode();
	config.left = left->getKey().asKeycode();
	config.right = right->getKey().asKeycode();
	config.jump = jump->getKey().asKeycode();
	config.sneak = sneak->getKey().asKeycode();
	config.sprint = sprint->getKey().asKeycode();
	config.dab = dab->getKey().asKeycode();
	config.view = changeView->getKey().asKeycode();
	// config.debug = debug->getKey().asKeycode();
}

std::unique_ptr<KeyMenu> KeyMenu::create() {
	auto btn = std::unique_ptr<KeyMenu>(new KeyMenu());
	btn->initialize();
	return btn;
}
