#include "MainMenuButton.hpp"

#include <glm/glm.hpp>

#include "gl/ResourceManager.hpp"
#include "ui/Component.hpp"
#include "ui/style/Property.hpp"
#include "ui/style/Type.hpp"

using namespace ui;
using namespace glm;

MainMenuButton::MainMenuButton(std::unique_ptr<Pane> mainComp, std::unique_ptr<Text> textComp)
  : mainComp(move(mainComp)), textComp(move(textComp))
{
  this->mainComp->add(this->textComp.get());
  add(this->mainComp.get());
}

void MainMenuButton::draw() {
  if(parent) mainComp->setSize(parent->getSize() - 2 * mainComp->getPadding());
  Button::draw();
}

std::unique_ptr<Button> MainMenuButton::create(std::string text) {
  auto font = ResourceManager::getFont("roboto");
  auto btn = new MainMenuButton(Pane::create(), Text::create(text, font));
  btn->initialize();
  btn->mainComp->setPadding({ 35, 20 });
  btn->textComp->setUseBaseline(true);
  btn->setProp(Pane::COLOR, vec4{ 0.f, 0.f, 0.f, .8f });
  btn->setProp(Text::COLOR, vec4(1.f));
  btn->setHoverStyle(make_prop(Text::COLOR, vec4(1.0, 0.0, 0.0, 1.0)));
  btn->setPressedStyle(make_prop(Text::COLOR, vec4(0.0, 1.0, 0.0, 1.0)));
  return std::unique_ptr<Button>(btn);
}
