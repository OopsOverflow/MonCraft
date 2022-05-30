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
	shadows = ComboBox::create({ "None", "Low", "High" }, clientConf.shadows);
	fov = RangeSlider::create(10, 170);
	renderDistH = RangeSlider::create(1, 40);
	renderDistV = RangeSlider::create(1, 20);
	vsync = Checkbox::create();

	fov->setValue((int)clientConf.fov);
	renderDistH->setValue(serverConf.renderDistH);
	renderDistV->setValue(serverConf.renderDistV);
	fullscreen->setChecked(clientConf.fullscreen);
	vsync->setChecked(clientConf.vsync);


	addLine("Plein écran", fullscreen.get());
	addLine("FOV", fov.get());
	addLine("Ombres", shadows.get());
	addLine("Distance horizontale", renderDistH.get());
	addLine("Distance verticale", renderDistV.get());
	addLine("VSync", vsync.get());

	// see application -> showParameters
	// fullscreen->onRelease([&]{ clientConf.fullscreen = this->fullscreen->getChecked(); });
	// vsync->onRelease([&]{ clientConf.vsync = this->vsync->getChecked(); });
	shadows->onRelease([&]{ clientConf.shadows = (int)this->shadows->getSelected(); });
	fov->onRelease([&]{ clientConf.fov = (float)this->fov->getValue(); });
	renderDistH->onRelease([&]{ serverConf.renderDistH = this->renderDistH->getValue(); });
	renderDistV->onRelease([&]{ serverConf.renderDistV = this->renderDistV->getValue(); });
	
}

GraphicsMenu::~GraphicsMenu() {
	auto& clientConf = Config::getClientConfig();
	auto& serverConf = Config::getServerConfig();

	clientConf.fov = (float)fov->getValue();
	serverConf.renderDistH = renderDistH->getValue();
	serverConf.renderDistV = renderDistV->getValue();
	clientConf.fullscreen = fullscreen->getChecked(); 
	clientConf.vsync = vsync->getChecked();
	clientConf.shadows = (uint8_t)shadows->getSelected();
}

std::unique_ptr<GraphicsMenu> GraphicsMenu::create() {
	auto menu = new GraphicsMenu();
	menu->initialize();
	return std::unique_ptr<GraphicsMenu>(menu);
}
