#include "GraphicsMenu.hpp"

#include <utility>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "interface/widgets/ComboBox.hpp"
#include "interface/widgets/MonCraftButton.hpp"
#include "interface/widgets/RangeSlider.hpp"
#include "gl/ResourceManager.hpp"
#include "ui/Button.hpp"
#include "ui/style/Type.hpp"
#include "save/ClientConfig.hpp"
#include "save/ServerConfig.hpp"

using namespace ui;
using namespace glm;


GraphicsMenu::GraphicsMenu()
{
	auto& clientConf = Config::getClientConfig();
	auto& serverConf = Config::getServerConfig();

	shadows = ComboBox::create({ "None", "Low", "High" }, 2);
	fov = RangeSlider::create(0, 180);
	renderDistH = RangeSlider::create(1, 40);
	renderDistV = RangeSlider::create(1, 20);

	fov->setValue(clientConf.fov);
	renderDistH->setValue(serverConf.renderDistH);
	renderDistV->setValue(serverConf.renderDistV);

	auto vsync = MonCraftButton::create("TODO checkbox");

	addLine("Plein écran", Slider::create());
	addLine("FOV", fov.get());
	addLine("Ombres", shadows.get());
	addLine("Distance horizontale", renderDistH.get());
	addLine("Distance verticale", renderDistV.get());
	addLine("VSync", move(vsync));
}

GraphicsMenu::~GraphicsMenu() {
	auto& clientConf = Config::getClientConfig();
	auto& serverConf = Config::getServerConfig();

	clientConf.fov = (float)fov->getValue();
	serverConf.renderDistH = renderDistH->getValue();
	serverConf.renderDistV = renderDistV->getValue();
}

std::unique_ptr<GraphicsMenu> GraphicsMenu::create() {
	auto menu = new GraphicsMenu();
	menu->initialize();
	return std::unique_ptr<GraphicsMenu>(menu);
}
