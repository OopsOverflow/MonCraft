#pragma once

#include "Pane.hpp"


namespace ui {

class Slider : public Component {

public:
  Slider();

  static const spec_t VALUE;

  virtual void onMouseIn(glm::ivec2 pos) override;
  virtual void onMouseOut(glm::ivec2 pos) override;
  virtual bool onMouseMove(glm::ivec2 pos) override;
  virtual bool onMousePressed(glm::ivec2 pos) override;
  virtual bool onMouseReleased(glm::ivec2 pos) override;
  virtual void draw() override;

	void setValue(float value);
	float getValue() const;

private:
  std::unique_ptr<Pane> track;
  std::unique_ptr<Pane> thumb;
  float value;

  void moveThumb(int pos);
  float posToValue(int pos) const;
  int valueToPos(float value) const;
};

}; // namespace ui