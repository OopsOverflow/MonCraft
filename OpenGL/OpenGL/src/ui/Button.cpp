#include "Button.hpp"

using namespace ui;
using namespace glm;

const spec_t Button::TEXT = MAKE_SPEC("Button::text", std::string);
const spec_t Button::TEXT_COLOR = MAKE_SPEC("Button::textColor", vec4);

Button::Button(Component* parent, std::string text, std::shared_ptr<const Font> font)
  : Pane(parent)
{
  textComp = std::make_unique<Text>(this, std::move(text), std::move(font));

  Button::getDefaultStyle()->apply(this);

  hover.setParent(Button::getDefaultStyle());
  hover.set(make_property(TEXT_COLOR, vec4(1.0, 0.0, 0.0, 1.0)));

  pressed.setParent(Button::getDefaultStyle());
  pressed.set(make_property(TEXT_COLOR, vec4(0.0, 1.0, 0.0, 1.0)));
}

void Button::setStyle(prop_t const& prop) {
  if(prop.spec == Button::TEXT) {
    setText(prop.value->get<std::string>());
  }
  else if(prop.spec == Button::TEXT_COLOR) {
    setTextColor(prop.value->get<vec4>());
  }
  else {
    Component::setStyle(prop);
  }
}

prop_t Button::getStyle(spec_t spec) const {
  if(spec == Button::TEXT) {
    return make_property(spec, getText());
  }
  if(spec == Button::TEXT_COLOR) {
    return make_property(spec, getColor());
  }
  else {
    return Component::getStyle(spec);
  }
}

style_const_t Button::getDefaultStyle() const {
  static style_const_t style = Style::make_style(
    Pane::getDefaultStyle(),
    make_property(Button::TEXT_COLOR, vec4(0.f, 0.f, 0.f, 1.f)),
    make_property(Component::PADDING, ivec2(10, 5))
  );

  return style;
}

#include "debug/Debug.hpp"

void Button::onMouseIn(glm::ivec2 pos) {
  std::cout << "in " << pos << std::endl;
  hover.apply(this);
}

void Button::onMouseOut(glm::ivec2 pos) {
  std::cout << "out " << pos << std::endl;
  if(isPressed()) pressed.revert(this);
  else hover.revert(this);
}

bool Button::onMousePressed(glm::ivec2 pos) {
  Component::onMousePressed(pos);
  hover.revert(this);
  pressed.apply(this);
  return true;
}

bool Button::onMouseReleased(glm::ivec2 pos) {
  bool res = Component::onMousePressed(pos);
  pressed.revert(this);
  hover.apply(this);
  return res;
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
