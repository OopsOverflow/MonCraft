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
  auto orig = getAbsoluteOrigin();
  vec3 pos(orig.x, orig.y, 0.f);
  shader->activate();

  vec3 startPos = pos;
  startPos.x -= font->characters.at(text.at(0)).bearing.x * 2; // COMBAK: why * 2 ?
  font->draw(text, startPos, fontSize, color);
  Component::draw();
}

ivec2 Text::getSize() const {
  ivec2 size{ 0, 0 };
  for(auto c : text) {
    auto const& ch = font->characters.at(c);
    size.x += (ch.advance >> 6) * fontSize;
    size.y = max(size.y, (int)(ch.size.y * fontSize));
  }
  return size;
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
  queueDraw();
}

float Text::getFontSize() const {
  return this->fontSize;
}

void Text::setFont(std::shared_ptr<const Font> font) {
  this->font = font;
  queueDraw();
}

std::shared_ptr<const Font> Text::getFont() const {
  return font;
}
