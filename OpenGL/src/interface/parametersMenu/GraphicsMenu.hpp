#pragma once

#include <memory>

#include "ui/Component.hpp"
#include "interface/widgets/ParamList.hpp"
#include "interface/widgets/ComboBox.hpp"
#include "interface/widgets/RangeSlider.hpp"
#include "interface/widgets/Checkbox.hpp"

class GraphicsMenu : public ParamList {

public:
	static std::unique_ptr<GraphicsMenu> create();
	~GraphicsMenu();

	std::shared_ptr<Checkbox> fullscreen;
	std::shared_ptr<Checkbox> vsync;
	std::shared_ptr<RangeSlider> msaa;
	std::shared_ptr<RangeSlider> fov;
	std::shared_ptr<RangeSlider> sensivity;
	std::shared_ptr<ComboBox> shadows;
	std::shared_ptr<RangeSlider> renderDistH, renderDistV;

private:
	GraphicsMenu();
};
