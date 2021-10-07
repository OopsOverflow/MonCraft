#include "Slider.hpp"
#include "Pane.hpp"

using namespace ui;
using namespace glm;

const spec_t Slider::VALUE = MAKE_SPEC("Slider::value", float);

Slider::Slider()
  : value(0)
{
  track = std::make_unique<Pane>();
  thumb = std::make_unique<Pane>();

  add(track.get());
  track->add(thumb.get());

  thumb->setColor({ 1, 0, 0, 1 });
  setSize(ivec2(100, 20));
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
  }
  return false;
}

void Slider::moveThumb(int pos) {
  auto tpos = thumb->getPosition();
  auto tsize = thumb->getAbsoluteSize();
  auto size = getAbsoluteSize();
  tpos.x = pos - tsize.x / 2.f;
  tpos.x = min(max(tpos.x, 0), size.x - tsize.x);

  thumb->setPosition({ tpos });
  value = posToValue(tpos.x);
}

bool Slider::onMousePressed(glm::ivec2 pos) {
  thumb->setColor({ 1, 0, 1, 1 });
  moveThumb(pos.x);
  return false;
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
  return value * (size.x - tsize.x) + tsize.x / 2.f;
}

void Slider::setValue(float value) {
  if(value < 0 || value > 1)
    throw std::runtime_error("value must be in range [0, 1]");
  moveThumb(valueToPos(value));
}

float Slider::getValue() const {
  return value;
}

void Slider::draw() {
  auto size = getSize();
  track->setSize(size);
  thumb->setSize({ size.y, size.y });

  Component::draw();
}
