#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "ui/Pane.hpp"
#include "ui/Component.hpp"


namespace ui {

class Slider : public Component {

protected:
  Slider();

public:
  static std::unique_ptr<Slider> create();

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
