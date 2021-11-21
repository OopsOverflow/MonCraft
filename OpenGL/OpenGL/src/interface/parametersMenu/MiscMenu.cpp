#include "MiscMenu.hpp"

#include <glm/glm.hpp>
#include <utility>

#include "gl/ResourceManager.hpp"
#include "ui/Button.hpp"
#include "ui/Component.hpp"
#include "ui/Image.hpp"
#include "ui/style/Type.hpp"

using namespace ui;
using namespace glm;

MiscMenu::MiscMenu()
{
	// mobileVersion = ParameterButton::create("Version Mobile", std::move(button));
	// pack_start(enableMobileVersionButton.get());
}

std::unique_ptr<MiscMenu> MiscMenu::create() {

	auto btn = new MiscMenu();
	btn->initialize();
	return std::unique_ptr<MiscMenu>(btn);
}
