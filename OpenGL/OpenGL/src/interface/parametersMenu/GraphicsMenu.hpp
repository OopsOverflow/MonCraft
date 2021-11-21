#pragma once

#include <memory>

#include "ui/Component.hpp"
#include "interface/widgets/ParamList.hpp"
#include "interface/widgets/ComboBox.hpp"
#include "interface/widgets/RangeSlider.hpp"

class GraphicsMenu : public ParamList {

public:
	static std::unique_ptr<GraphicsMenu> create();
	~GraphicsMenu();

private:
	GraphicsMenu();

	std::unique_ptr<RangeSlider> fov;
	std::unique_ptr<ComboBox> shadows;
	std::unique_ptr<RangeSlider> renderDistH, renderDistV;
};
