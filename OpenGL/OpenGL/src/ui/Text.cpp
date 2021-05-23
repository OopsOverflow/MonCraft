#include "Text.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;
using namespace glm;


Text::Text(Component* parent, std::string text, std::shared_ptr<const Font> font)
  : Component(parent),
    text(std::move(text)),
    color(0.f, 0.f, 0.f, 1.f),
    fontSize(1.f),
    font(std::move(font))
{
  shader = ResourceManager::getShader("font");
}

void Text::draw() {
  computeSize();
  auto orig = getAbsoluteOrigin();
  vec3 pos(orig.x, orig.y, 0.f);
  shader->activate();
  pos.x -= font->characters.at(text.at(0)).bearing.x * fontSize;
  font->draw(text, pos, fontSize, color);
  Component::draw();
}

void Text::computeSize() {
  ivec2 size{ 0, 0 };

  if(text.size() != 0) {

    for(auto c : text) {
      auto const& ch = font->characters.at(c);
      size.x += ch.advance * fontSize;
      size.y = max(size.y, (int)(ch.size.y * fontSize));
    }

    auto const& first = font->characters.at(*text.begin());
    auto const& last = font->characters.at(*--text.end());
    size.x -= first.bearing.x * fontSize;
    size.x -= (last.advance - last.size.x - last.bearing.x) * fontSize;
  }

  setSize(size);
}

void Text::setColor(glm::vec4 color) {
  this->color = color;
  queueDraw();
}

glm::vec4 Text::getColor() const {
  return color;
}

void Text::setText(std::string text) {
  this->text = text;
  queueDraw();
}

std::string Text::getText() const {
  return this->text;
}

void Text::setFontSize(float fontSize) {
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
