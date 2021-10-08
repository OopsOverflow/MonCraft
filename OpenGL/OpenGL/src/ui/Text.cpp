#include "Text.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;
using namespace glm;

MAKE_TYPE(std::shared_ptr<const Font>);
const spec_t Text::COLOR        = MAKE_SPEC("Text::color", vec4);
const spec_t Text::FONT_SIZE    = MAKE_SPEC("Text::fontSize", float);
const spec_t Text::FONT         = MAKE_SPEC("Text::font", std::shared_ptr<const Font>);
const spec_t Text::USE_BASELINE = MAKE_SPEC("Text::useBaseline", bool);

Text::Text(std::string text, std::shared_ptr<const Font> font)
  : text(std::move(text)),
    color(0.f, 0.f, 0.f, 1.f), fontSize(1.f),
    font(std::move(font)), shader(ResourceManager::getShader("font")),
    useBaseline(true), baselineOffset(0)
{
  computeSize();
}

void Text::setProperty(prop_t prop) {
  if(prop.spec == Text::COLOR) {
    setColor(prop.value->get<vec4>());
  }
  else if(prop.spec == Text::FONT_SIZE) {
    setFontSize(prop.value->get<float>());
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
    return make_property(spec, getColor());
  }
  else if(spec == Text::FONT_SIZE) {
    return make_property(spec, getFontSize());
  }
  else if(spec == Text::USE_BASELINE) {
    return make_property(spec, getUseBaseline());
  }
  else {
    return Component::getProperty(spec);
  }
}

style_const_t Text::getDefaultStyle() const {
  static style_const_t style = Style::make_style(
    Component::getDefaultStyle(),
    make_property(Text::COLOR, vec4(0.f, 0.f, 0.f, 1.f)),
    make_property(Text::FONT_SIZE, 1.f),
    make_property(Text::USE_BASELINE, true)
  );

  return style;
}

void Text::draw() {
  auto orig = getAbsoluteOrigin();
  vec3 pos(orig.x, orig.y, 0.f);
  shader->activate();
  if(text.size() > 0) {
    pos.x -= font->characters.at(text.at(0)).bearing.x * fontSize;
    if(!useBaseline) pos.y += baselineOffset;
    font->draw(text, pos, fontSize, color);
  }
  Component::draw();
}

void Text::computeSize() {
  ivec2 size{ 0, 0 };
  int off = 0;

  if(text.size() > 0) {

    for(auto c : text) {
      auto const& ch = font->characters.at(c);
      size.x += ch.advance;
      size.y = max(size.y, ch.bearing.y);
      off = max(off, ch.size.y - ch.bearing.y);
    }

    auto const& first = font->characters.at(*text.begin());
    auto const& last = font->characters.at(*--text.end());
    size.x -= first.bearing.x;
    size.x -= (last.advance - last.size.x - last.bearing.x);
  }

  if(!useBaseline) size.y += off;
  size.x *= fontSize;
  size.y *= fontSize;
  baselineOffset = off * fontSize;
  setSize(size);
}

void Text::setColor(glm::vec4 color) {
  if(color == this->color) return;
  this->color = color;
  queueDraw();
}

glm::vec4 Text::getColor() const {
  return color;
}

void Text::setText(std::string text) {
  this->text = text;
  computeSize();
  queueDraw();
}

std::string Text::getText() const {
  return this->text;
}

void Text::setFontSize(float fontSize) {
  if(fontSize == this->fontSize) return;
  this->fontSize = fontSize;
  computeSize();
}

float Text::getFontSize() const {
  return this->fontSize;
}

void Text::setFont(std::shared_ptr<const Font> font) {
  this->font = font;
  computeSize();
}

std::shared_ptr<const Font> Text::getFont() const {
  return font;
}

void Text::setUseBaseline(bool useBaseline) {
  if(useBaseline == this->useBaseline) return;
  this->useBaseline = useBaseline;
  computeSize();
}

bool Text::getUseBaseline() const {
  return useBaseline;
}
