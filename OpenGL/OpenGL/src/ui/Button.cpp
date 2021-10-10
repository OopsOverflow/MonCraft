#include "Button.hpp"
#include "Image.hpp"

using namespace ui;
using namespace glm;

Button::Button(std::unique_ptr<Component> comp, std::string text, std::shared_ptr<const Font> font)
  : mainComp(std::move(comp)),
    hover(std::make_shared<Style>()),
    pressed(std::make_shared<Style>())
{
  textComp = Text::create(std::move(text), std::move(font));
  mainComp->add(textComp.get());
  add(mainComp.get());

  hover->set(make_property(Text::COLOR, vec4(1.0, 0.0, 0.0, 1.0)));

  pressed->setParent(hover);
  pressed->set(make_property(Text::COLOR, vec4(0.0, 1.0, 0.0, 1.0)));

  textComp->setUseBaseline(false); // TODO: non-heritable styles ?
  mainComp->setPadding(ivec2(10, 5));
}

std::unique_ptr<Button> Button::createPaneButton(std::string text, std::shared_ptr<const Font> font) {
  auto btn = new Button(Pane::create(), text, font);
  btn->initialize();
  return std::unique_ptr<Button>(btn);
}

std::unique_ptr<Button> Button::createImageButton(glm::ivec2 offset, glm::ivec2 size, std::string text, std::shared_ptr<const Font> font) {
  auto btn = new Button(Image::create(offset, size), text, font);
  btn->initialize();
  return std::unique_ptr<Button>(btn);
}

void Button::onclick(std::function<void()> callback) {
  this->clickCallback = callback;
}

void Button::setHoverStyle(prop_t prop) {
  hover->set(prop);
}

void Button::setPressedStyle(prop_t prop) {
  pressed->set(prop);
}

#include "debug/Debug.hpp"

void Button::onMouseIn(glm::ivec2 pos) {
  hover->apply(this);
}

void Button::onMouseOut(glm::ivec2 pos) {
  if(isPressed()) pressed->revert(this);
  hover->revert(this);
}

bool Button::onMousePressed(glm::ivec2 pos) {
  pressed->apply(this);
  if(clickCallback) clickCallback();
  return true;
}

bool Button::onMouseReleased(glm::ivec2 pos) {
  pressed->revert(this);
  return false;
}

void Button::setText(std::string text) {
  textComp->setText(std::move(text));
}

std::string Button::getText() const {
  return textComp->getText();
}

void Button::setTextColor(glm::vec4 color) {
  textComp->setColor(color);
}

glm::vec4 Button::getTextColor() const {
  return textComp->getColor();
}

void Button::setFontSize(float fontSize) {
  textComp->setFontSize(fontSize);
}

float Button::getFontSize() const {
  return textComp->getFontSize();
}
