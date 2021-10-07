#pragma once

#include "Component.hpp"
#include "Pane.hpp"
#include "Text.hpp"

#include <functional>

namespace ui {

class Input : public Pane {

public:
  Input(std::string text, std::shared_ptr<const Font> font);

  static const spec_t TEXT;
  static const spec_t TEXT_COLOR;
  virtual void setProperty(prop_t prop) override;
  virtual prop_t getProperty(spec_t spec) const override;
  virtual style_const_t getDefaultStyle() const override;

  void onChange(std::function<void()> callback);

  // forward methods from ui::Text
  void setText(std::string text);
  std::string getText() const;
  void setTextColor(glm::vec4 color);
  glm::vec4 getTextColor() const;

protected:
  void onMouseIn(glm::ivec2 pos) override;
  void onMouseOut(glm::ivec2 pos) override;
  bool onActivate() override;
  void onDeactivated() override;
  bool onMousePressed(glm::ivec2 pos) override;
  void onKeyPressed(Key k) override;

private:
  std::unique_ptr<Text> textComp;
  Style hover;
  Style active;
  std::function<void()> changeCallback;
};

} // namespace ui
