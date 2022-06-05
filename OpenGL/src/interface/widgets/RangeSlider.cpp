#include "RangeSlider.hpp"

#include <string>
#include <memory>
#include <math.h>

#include "ui/Slider.hpp"
#include "ui/Text.hpp"
#include "ui/Box.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;

RangeSlider::RangeSlider(int min, int max, int increment)
  : min(min), max(max), increment(increment),
    font(ResourceManager::getFont("roboto"))
{
  auto box = Box::create();
  slider = Slider::create();
  text = Text::create("0", font);

  box->setOrientation(Box::Orientation::HORIZONTAL);
  box->setGap(10);
  box->pack_end(slider.get());
  box->pack_end(text.get());
  add(move(box));
}

void RangeSlider::draw() {
  std::string str = std::to_string(getValue());
  text->setText(str);

  ui::Component::draw();
}

std::unique_ptr<RangeSlider> RangeSlider::create(int min, int max, int increment) {
  auto slider = std::unique_ptr<RangeSlider>(new RangeSlider(min, max, increment));
  slider->initialize();
  return slider;
}

int RangeSlider::getValue() const {
  // TODO increment
  return (int)round(slider->getValue() * (max - min) + min);
}

void RangeSlider::setValue(int val) {
  slider->setValue(float(val - min) / float(max - min));
}
