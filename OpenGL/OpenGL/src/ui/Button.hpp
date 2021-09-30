#pragma once

#include "Component.hpp"
#include "Pane.hpp"
#include "Text.hpp"

#include <functional>

namespace ui {

class Button : public Pane {

public:
  Button(std::string text, std::shared_ptr<const Font> font);

  static const spec_t TEXT;
  static const spec_t TEXT_COLOR;
  virtual void setStyle(prop_t const& prop) override;
  virtual prop_t getStyle(spec_t spec) const override;
  virtual style_const_t getDefaultStyle() const override;

  void onclick(std::function<void()> callback);

  // forward methods from ui::Text
  void setText(std::string text);
  std::string getText() const;
  void setTextColor(glm::vec4 color);
  glm::vec4 getTextColor() const;

protected:
  void onMouseIn(glm::ivec2 pos) override;
  bool onMousePressed(glm::ivec2 pos) override;
  bool onMouseReleased(glm::ivec2 pos) override;
  void onMouseOut(glm::ivec2 pos) override;

private:
  std::unique_ptr<Text> textComp;
  Style hover;
  Style pressed;
  std::function<void()> clickCallback;
};

} // namespace ui
