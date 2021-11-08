#include "Input.hpp"

#include <glm/glm.hpp>

#include "ui/Text.hpp"
#include "ui/style/Property.hpp"
#include "ui/style/Type.hpp"

class Font;

using namespace ui;
using namespace glm;

Input::Input(std::string text, std::shared_ptr<const Font> font)
  : hover(std::make_shared<Style>()),
    active(std::make_shared<Style>())
{
  textComp = Text::create(std::move(text), std::move(font));
  add(textComp.get());

  hover->setParent(getOwnStylesheet());
  hover->set(make_prop(Text::COLOR, vec4(1.0, 0.0, 0.0, 1.0)));

  active->setParent(hover);
  active->set(make_prop(Text::COLOR, vec4(0.0, 1.0, 0.0, 1.0)));
}

std::unique_ptr<Input> Input::create(std::string text, std::shared_ptr<const Font> font) {
  auto comp = std::unique_ptr<Input>(new Input(text, font));
  comp->initialize();
  return comp;
}

void Input::onChange(std::function<void()> callback) {
  this->changeCallback = callback;
}

void Input::onMouseIn(glm::ivec2 pos) {
  if(!isActive()) setStylesheet(hover);
}

void Input::onMouseOut(glm::ivec2 pos) {
  if(!isActive()) setStylesheet(getOwnStylesheet());
}

bool Input::onActivate() {
  if(isHover()) setStylesheet(active);
  return true;
}

void Input::onDeactivated() {
  if(isHover()) setStylesheet(hover);
  else setStylesheet(getOwnStylesheet());
}

bool Input::onMousePressed(glm::ivec2 pos) {
  return true;
}

void Input::onKeyPressed(Key k) {
  auto text = getText();
  if(k.isPrintable()) {
    setText(text + k.asChar());
  }
  else if(k.isBackspace() && text.size() != 0) {
    text.pop_back();
    setText(text);
  }
  else if(k.isEsc()) {
    unfocus();
  }
}

void Input::setText(std::string text) {
  textComp->setText(std::move(text));
}

std::string Input::getText() const {
  return textComp->getText();
}

void Input::setTextColor(glm::vec4 color) {
  textComp->setColor(color);
}

glm::vec4 Input::getTextColor() const {
  return textComp->getColor();
}
