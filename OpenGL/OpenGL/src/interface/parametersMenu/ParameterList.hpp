#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ui/Box.hpp"
#include "ui/Button.hpp"
#include "ui/Text.hpp"

class Font;

class ParameterList : public ui::Box {

public:
	static std::unique_ptr<Box> create(std::string parameterName, std::vector<std::string> paramNames, size_t value);

	virtual void draw() override;



protected:
	ParameterList(std::string parameterName, std::shared_ptr<const Font> font, std::vector<std::string> paramNames, size_t value);

private:
	std::unique_ptr<ui::Text> title;
	std::unique_ptr<ui::Text> parameter;
	std::unique_ptr<ui::Button> leftButton;
	std::unique_ptr<ui::Button> rightButton;

	size_t paramID;
	std::vector<std::string> paramNames;
};
