#pragma once

#include <memory>

#include "ui/Button.hpp"
#include "ui/Component.hpp"
#include "ui/Pane.hpp"
#include "ui/Text.hpp"

class GameMenu : public ui::Component
{
protected : 
	GameMenu();
	virtual bool onMousePressed(glm::ivec2 pos) override;

public:
	static std::unique_ptr<GameMenu> create();

	std::shared_ptr<ui::Button> continueButton;
	std::shared_ptr<ui::Button> parameterButton;
	std::shared_ptr<ui::Button> quitButton;

};