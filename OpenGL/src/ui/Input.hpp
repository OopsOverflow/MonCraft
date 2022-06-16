#pragma once

#include <functional>
#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "ui/Pane.hpp"
#include "ui/Text.hpp"
#include "ui/Key.hpp"
#include "ui/style/Style.hpp"

class Font;

namespace ui {

class Input : public Pane {

protected:
  Input(std::string text, std::shared_ptr<const Font> font);

public:
  static std::unique_ptr<Input> create(std::string text, std::shared_ptr<const Font> font);

  // virtual void setProperty(prop_t prop) override;
  // virtual prop_t getProperty(spec_t spec) const override;
  // virtual style_const_t getDefaultStyle() const override;

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
  std::shared_ptr<Text> textComp;
  style_t hover;
  style_t active;
  std::function<void()> changeCallback;
};

} // namespace ui
