#include "Button.hpp"

#include <glm/glm.hpp>
#include <utility>

#include "ui/Component.hpp"
#include "ui/Image.hpp"
#include "ui/Pane.hpp"
#include "ui/Text.hpp"
#include "ui/style/Property.hpp"
#include "ui/style/Type.hpp"

class Font;

using namespace ui;
using namespace glm;

Button::Button()
  : hover(std::make_shared<Style>()),
    pressed(std::make_shared<Style>())
{
  hover->setParent(getOwnStylesheet());
  pressed->setParent(hover);
}

std::unique_ptr<Button> Button::createPaneButton(std::string text, std::shared_ptr<const Font> font) {
  auto comp = Pane::create();
  auto textComp = Text::create(std::move(text), std::move(font));
  auto btn = new Button();

  comp->setPadding(ivec2(10, 5));
  textComp->setUseBaseline(false);

  btn->setHoverStyle(make_prop(Text::COLOR, vec4(1.0, 0.0, 0.0, 1.0)));
  btn->setPressedStyle(make_prop(Text::COLOR, vec4(0.0, 1.0, 0.0, 1.0)));

  comp->add(move(textComp));
  btn->add(move(comp));

  btn->initialize();
  return std::unique_ptr<Button>(btn);
}

std::unique_ptr<Button> Button::createImageButton(glm::ivec2 offset, glm::ivec2 size, std::string text, std::shared_ptr<const Font> font) {
  auto comp = Image::create(offset, size);
  auto textComp = Text::create(std::move(text), std::move(font));
  auto btn = new Button();

  comp->setPadding(ivec2(10, 5));
  textComp->setUseBaseline(false);

  btn->setHoverStyle(make_prop(Text::COLOR, vec4(1.0, 0.0, 0.0, 1.0)));
  btn->setPressedStyle(make_prop(Text::COLOR, vec4(0.0, 1.0, 0.0, 1.0)));

  comp->add(move(textComp));
  btn->add(move(comp));

  btn->initialize();
  return std::unique_ptr<Button>(btn);
}

std::unique_ptr<Button> Button::create() {
  auto btn = new Button();

  btn->initialize();
  return std::unique_ptr<Button>(btn);
}

void Button::onClick(std::function<void()> callback) {
  this->clickCallback = callback;
}

void Button::onRelease(std::function<void()> callback) {
  this->releaseCallback = callback;
}

void Button::setHoverStyle(prop_t prop) {
  hover->set(prop);
}

void Button::setPressedStyle(prop_t prop) {
  pressed->set(prop);
}

void Button::onMouseIn(glm::ivec2 pos) {
  setStylesheet(hover);
}

void Button::onMouseOut(glm::ivec2 pos) {
  setStylesheet(getOwnStylesheet());
}

bool Button::onMousePressed(glm::ivec2 pos) {
  setStylesheet(pressed);
  if(clickCallback) clickCallback();
  return true;
}

bool Button::onMouseReleased(glm::ivec2 pos) {
  setStylesheet(hover);
  if(releaseCallback) releaseCallback();
  return false;
}
