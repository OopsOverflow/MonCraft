#include "MainMenuButton.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;
using namespace glm;

MainMenuButton::MainMenuButton(std::unique_ptr<Component> comp, std::string text, std::shared_ptr<const Font> font)
  : Button(move(comp), move(text), move(font))
{}

void MainMenuButton::draw() {
  if(parent) mainComp->setSize(parent->getSize() - 2 * mainComp->getPadding());
  Button::draw();
}

std::unique_ptr<Button> MainMenuButton::create(std::string text) {
  auto font = ResourceManager::getFont("roboto");
  auto btn = new MainMenuButton(Pane::create(), text, font);
  btn->initialize();
  btn->mainComp->setPadding({ 35, 20 });
  btn->textComp->setUseBaseline(true);
  btn->setProp(Pane::COLOR, vec4{ 0.f, 0.f, 0.f, .8f });
  btn->setProp(Text::COLOR, vec4(1.f));
  // btn->setHoverStyle(make_prop(Pane::COLOR, vec4{ 1.f, 0.f, 0.f, .5f }));
  return std::unique_ptr<Button>(btn);
}
