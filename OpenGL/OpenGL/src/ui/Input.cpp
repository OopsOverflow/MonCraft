#include "Input.hpp"

using namespace ui;
using namespace glm;

const spec_t Input::TEXT = MAKE_SPEC("Input::text", std::string);
const spec_t Input::TEXT_COLOR = MAKE_SPEC("Input::textColor", vec4);

Input::Input(std::string text, std::shared_ptr<const Font> font)
{
  textComp = std::make_unique<Text>(std::move(text), std::move(font));
  add(textComp.get());



  hover.setParent(Input::getDefaultStyle());
  hover.set(make_property(TEXT_COLOR, vec4(1.0, 0.0, 0.0, 1.0)));

  active.setParent(Input::getDefaultStyle());
  active.set(make_property(TEXT_COLOR, vec4(0.0, 1.0, 0.0, 1.0)));
}

void Input::onChange(std::function<void()> callback) {
  this->changeCallback = callback;
}

void Input::setProperty(prop_t prop) {
  if(prop.spec == Input::TEXT) {
    setText(prop.value->get<std::string>());
  }
  else if(prop.spec == Input::TEXT_COLOR) {
    setTextColor(prop.value->get<vec4>());
  }
  else {
    Component::setProperty(prop);
  }
}

prop_t Input::getProperty(spec_t spec) const {
  if(spec == Input::TEXT) {
    return make_property(spec, getText());
  }
  if(spec == Input::TEXT_COLOR) {
    return make_property(spec, getColor());
  }
  else {
    return Component::getProperty(spec);
  }
}

style_const_t Input::getDefaultStyle() const {
  static style_const_t style = Style::make_style(
    Pane::getDefaultStyle(),
    make_property(Input::TEXT_COLOR, vec4(0.f, 0.f, 0.f, 1.f)),
    make_property(Component::PADDING, ivec2(10, 5))
  );

  return style;
}

#include "debug/Debug.hpp"

void Input::onMouseIn(glm::ivec2 pos) {
  if(!isActive()) hover.apply(this);
}

void Input::onMouseOut(glm::ivec2 pos) {
  if(!isActive()) hover.revert(this);
}

bool Input::onActivate() {
  if(isHover()) hover.revert(this);
  active.apply(this);
  return true;
}

void Input::onDeactivated() {
  active.revert(this);
  if(isHover()) hover.apply(this);
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
