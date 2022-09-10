#include "Text.hpp"

#include <gl/UTF8Iter.hpp>
#include <glm/glm.hpp>

#include "gl/ResourceManager.hpp"
#include "gl/Font.hpp"
#include "gl/Shader.hpp"
#include "ui/Component.hpp"
#include "ui/style/Type.hpp"
#include "ui/style/Value.hpp"

using namespace ui;
using namespace glm;

MAKE_TYPE(std::shared_ptr<const Font>);
const spec_t Text::COLOR        = MAKE_SPEC_INHERIT("Text::color", vec4);
const spec_t Text::FONT_SIZE    = MAKE_SPEC_INHERIT("Text::fontSize", float);
const spec_t Text::FONT         = MAKE_SPEC_INHERIT("Text::font", std::shared_ptr<const Font>);
const spec_t Text::USE_BASELINE = MAKE_SPEC("Text::useBaseline", bool);

Text::Text(std::string text, std::shared_ptr<const Font> font)
  : text(text),
    shader(ResourceManager::getShader("font")),
    baselineOffset(0)
{
  computeSize();
}

std::unique_ptr<Text> Text::create(std::string text, std::shared_ptr<const Font> font) {
  auto comp = std::unique_ptr<Text>(new Text(text, font));
  comp->initialize();
  return comp;
}

void Text::setProperty(prop_t prop) {
  if(prop.spec == Text::COLOR) {
    setColor(prop.value->get<vec4>());
  }
  else if(prop.spec == Text::FONT_SIZE) {
    setFontSize(prop.value->get<float>());
  }
  else if(prop.spec == Text::FONT) {
    setFont(prop.value->get<std::shared_ptr<const Font>>());
  }
  else if(prop.spec == Text::USE_BASELINE) {
    setUseBaseline(prop.value->get<bool>());
  }
  else {
    Component::setProperty(prop);
  }
}

prop_t Text::getProperty(spec_t spec) const {
  if(spec == Text::COLOR) {
    return make_prop(spec, getColor());
  }
  else if(spec == Text::FONT_SIZE) {
    return make_prop(spec, getFontSize());
  }
  else if(spec == Text::FONT) {
    return make_prop(spec, getFont());
  }
  else if(spec == Text::USE_BASELINE) {
    return make_prop(spec, getUseBaseline());
  }
  else {
    return Component::getProperty(spec);
  }
}

style_const_t Text::getDefaultStyle() const {
  static style_const_t style = Style::make_style(
    Component::getDefaultStyle(),
    Text::COLOR, vec4(0.f, 0.f, 0.f, 1.f),
    Text::FONT_SIZE, 1.f,
    Text::FONT, ResourceManager::getFont("roboto"),
    Text::USE_BASELINE, true
  );

  return style;
}

void Text::draw() {
  auto font = getFont();
  auto orig = getAbsoluteOrigin();
  vec3 pos(orig.x, orig.y, 0.f);
  shader->activate();
  if(text.size() > 0) {
    pos.x -= font->getChar(text.at(0)).bearing.x * getFontSize();
    if(!getUseBaseline()) pos.y += baselineOffset;
    font->draw(text, pos, getFontSize(), getColor());
  }
  Component::draw();
}

void Text::computeSize() {
  auto font = getFont();
  auto fontSize = getFontSize();
  ivec2 size{ 0, 0 };
  int off = 0;

  if(text.size() > 0) {

    for(char32_t c : UTF8StringAdaptator(text)) {
      auto const& ch = font->getChar(c);
      size.x += ch.advance;
      size.y = max(size.y, ch.bearing.y);
      off = max(off, ch.size.y - ch.bearing.y);
    }

    auto const& first = font->getChar(*text.begin());
    auto const& last = font->getChar(*--text.end());
    size.x -= first.bearing.x;
    size.x -= (last.advance - last.size.x - last.bearing.x);
  }

  if(!getUseBaseline()) size.y += off;
  size.x = (int)(size.x * fontSize);
  size.y = (int)(size.y * fontSize);
  baselineOffset = off * fontSize;
  setSize(size);
}

void Text::setColor(vec4 color) {
  setStyle(COLOR, color);
}

vec4 Text::getColor() const {
  return getStyle<vec4>(COLOR);
}

void Text::setText(std::string text) {
  this->text = text;
  computeSize();
}

std::string Text::getText() const {
  return text;
}

void Text::setFontSize(float fontSize) {
  setStyle(FONT_SIZE, fontSize);
  computeSize();
}

float Text::getFontSize() const {
  return getStyle<float>(FONT_SIZE);
}

void Text::setFont(std::shared_ptr<const Font> font) {
  setStyle(FONT, font);
  computeSize();
}

std::shared_ptr<const Font> Text::getFont() const {
  return getStyle<std::shared_ptr<const Font>>(FONT);
}

void Text::setUseBaseline(bool useBaseline) {
  setStyle(USE_BASELINE, useBaseline);
  computeSize();
}

bool Text::getUseBaseline() const {
  return getStyle<bool>(USE_BASELINE);
}
