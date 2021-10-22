#pragma once

#include "ui/Button.hpp"

class MainMenuButton : public ui::Button {

public:
  static std::unique_ptr<Button> create(std::string text);

  virtual void draw() override;

protected:
  MainMenuButton(std::unique_ptr<ui::Pane> mainComp, std::unique_ptr<ui::Text> textComp);
  std::unique_ptr<ui::Pane> mainComp;
  std::unique_ptr<ui::Text> textComp;
};
