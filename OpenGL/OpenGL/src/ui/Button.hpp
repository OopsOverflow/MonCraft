#pragma once

#include "Component.hpp"
#include "Pane.hpp"
#include "Text.hpp"

#include <functional>

namespace ui {

class Button : public Component {

protected:
  Button(std::unique_ptr<Component> comp, std::string text, std::shared_ptr<const Font> font);

public:
  static std::unique_ptr<Button> createPaneButton(std::string text, std::shared_ptr<const Font> font);
  static std::unique_ptr<Button> createImageButton(glm::ivec2 offset, glm::ivec2 size, std::string text, std::shared_ptr<const Font> font);

  void setHoverStyle(prop_t prop);
  void setPressedStyle(prop_t prop);


  // virtual void setProperty(prop_t prop) override;
  // virtual prop_t getProperty(spec_t spec) const override;
  // virtual style_const_t getDefaultStyle() const override;

  void onclick(std::function<void()> callback);

  // forward methods from ui::Text
  void setText(std::string text);
  std::string getText() const;
  void setTextColor(glm::vec4 color);
  glm::vec4 getTextColor() const;
  void setFontSize(float fontSize);
  float getFontSize() const;

protected:
  void onMouseIn(glm::ivec2 pos) override;
  bool onMousePressed(glm::ivec2 pos) override;
  bool onMouseReleased(glm::ivec2 pos) override;
  void onMouseOut(glm::ivec2 pos) override;

  std::unique_ptr<Component> mainComp;
  std::unique_ptr<Text> textComp;
  style_t hover;
  style_t pressed;

private:
  std::function<void()> clickCallback;
};

} // namespace ui
