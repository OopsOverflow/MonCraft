#pragma once

#include "Component.hpp"
#include "Pane.hpp"
#include "Text.hpp"

#include <functional>

namespace ui {

class Button : public Component {

public:
  Button(std::unique_ptr<Component> comp, std::string text, std::shared_ptr<const Font> font);
  Button(std::string text, std::shared_ptr<const Font> font);

  static const spec_t TEXT;
  static const spec_t TEXT_COLOR;
  static const spec_t FONT_SIZE;
  virtual void setStyle(prop_t const& prop) override;
  virtual prop_t getStyle(spec_t spec) const override;
  virtual style_const_t getDefaultStyle() const override;

  void onclick(std::function<void()> callback);

  // forward methods from ui::Text
  void setText(std::string text);
  std::string getText() const;
  void setTextColor(glm::vec4 color);
  glm::vec4 getTextColor() const;
  void setFontSize(float fontSize);
  float getFontSize() const;

  Style& hoverStyle();
  Style& pressedStyle();

protected:
  void onMouseIn(glm::ivec2 pos) override;
  bool onMousePressed(glm::ivec2 pos) override;
  bool onMouseReleased(glm::ivec2 pos) override;
  void onMouseOut(glm::ivec2 pos) override;

private:
  std::unique_ptr<Component> mainComp;
  std::unique_ptr<Text> textComp;
  Style hover;
  Style pressed;
  std::function<void()> clickCallback;
};

} // namespace ui
