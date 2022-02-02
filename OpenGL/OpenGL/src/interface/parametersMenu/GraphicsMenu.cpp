#include "GraphicsMenu.hpp"

#include <utility>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "interface/widgets/ComboBox.hpp"
#include "interface/widgets/MonCraftButton.hpp"
#include "interface/widgets/RangeSlider.hpp"
#include "interface/widgets/Checkbox.hpp"
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

	fullscreen = Checkbox::create();
	shadows = ComboBox::create({ "None", "Low", "High" }, 2);
	fov = RangeSlider::create(10, 170);
	renderDistH = RangeSlider::create(1, 40);
	renderDistV = RangeSlider::create(1, 20);
	vsync = Checkbox::create();

	fov->setValue(clientConf.fov);
	renderDistH->setValue(serverConf.renderDistH);
	renderDistV->setValue(serverConf.renderDistV);

	addLine("Plein écran", fullscreen.get());
	addLine("FOV", fov.get());
	addLine("Ombres", shadows.get());
	addLine("Distance horizontale", renderDistH.get());
	addLine("Distance verticale", renderDistV.get());
	addLine("VSync", vsync.get());
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
