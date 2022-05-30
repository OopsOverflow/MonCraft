#pragma once

#include <memory>

#include "ui/Button.hpp"
#include "ui/Slider.hpp"
#include "ui/Text.hpp"

class RangeSlider : public ui::Button {

protected:
  RangeSlider(int min, int max, int increment);
  virtual void draw() override;

public:
  int getValue() const;
  void setValue(int val);
  static std::unique_ptr<RangeSlider> create(int min, int max, int increment=1);

private:
  int min, max, increment;
  std::unique_ptr<ui::Slider> slider;
  std::unique_ptr<ui::Text> text;
  std::shared_ptr<const Font> font;
};
