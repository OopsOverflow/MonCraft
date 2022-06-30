#pragma once

#include <memory>
#include <string>

#include "ui/Component.hpp"
#include "ui/Grid.hpp"
#include "gl/Font.hpp"

class ParamList : public ui::Component {

public:
	using ui::Component::initialize;
	static std::unique_ptr<ParamList> create();

	void addLine(std::string name, std::unique_ptr<Component> comp);
	void addLine(std::string name, Component* comp);

protected:
	ParamList();

private:
	std::unique_ptr<ui::Grid> grid;
	std::shared_ptr<const Font> font;
	int lastRow;
};
