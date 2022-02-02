#include "Slider.hpp"

#include <stdexcept>
#include <glm/glm.hpp>

#include "ui/Pane.hpp"
#include "ui/style/Type.hpp"

using namespace ui;
using namespace glm;

Slider::Slider()
  : value(0)
{
  track = Pane::create();
  thumb = Pane::create();

  add(track.get());
  track->add(thumb.get());

  thumb->setColor({ 1, 0, 0, 1 });
  setSize(ivec2(100, 20));
}

std::unique_ptr<Slider> Slider::create() {
  auto comp = std::unique_ptr<Slider>(new Slider());
  comp->initialize();
  return comp;
}

void Slider::onMouseIn(glm::ivec2 pos) {
  Component::onMouseIn(pos);
  thumb->setColor({ 0, 1, 1, 1 });
}

void Slider::onMouseOut(glm::ivec2 pos) {
  Component::onMouseOut(pos);
  thumb->setColor({ 1, 0, 0, 1 });
}

bool Slider::onMouseMove(glm::ivec2 pos) {
  if(isPressed()) {
    moveThumb(pos.x);
    value = posToValue(thumb->getPosition().x);
  }
  return false;
}

void Slider::moveThumb(int pos) {
  auto tpos = thumb->getPosition();
  auto tsize = thumb->getAbsoluteSize();
  auto size = getAbsoluteSize();
  tpos.x = pos - tsize.x / 2;
  tpos.x = min(max(tpos.x, 0), size.x - tsize.x);

  thumb->setPosition({ tpos });
}

bool Slider::onMousePressed(glm::ivec2 pos) {
  thumb->setColor({ 1, 0, 1, 1 });
  moveThumb(pos.x);
  value = posToValue(thumb->getPosition().x);
  return true;
}

bool Slider::onMouseReleased(glm::ivec2 pos) {
  thumb->setColor({ 0, 1, 1, 1 });
  return false;
}

float Slider::posToValue(int pos) const {
  auto size = getAbsoluteSize();
  auto tsize = thumb->getAbsoluteSize();
  return (float)pos / (size.x - tsize.x);
}

int Slider::valueToPos(float value) const {
  auto size = getAbsoluteSize();
  auto tsize = thumb->getAbsoluteSize();
  return (int)(value * (size.x - tsize.x) + tsize.x / 2.f);
}

void Slider::setValue(float value) {
  if(value < 0 || value > 1)
    throw std::runtime_error("value must be in range [0, 1]");
  moveThumb(valueToPos(value));
  this->value = value;
}

float Slider::getValue() const {
  return value;
}

void Slider::draw() {
  auto size = getSize();
  track->setSize(size);
  thumb->setSize({ size.y, size.y });
  moveThumb(valueToPos(value));

  Component::draw();
}
