#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "Component.hpp"
#include "gl/Font.hpp"

namespace ui {

class Text : public Component {

public:
  Text(std::string text, std::shared_ptr<const Font> font);

  void draw() override;

  static const spec_t COLOR;
  static const spec_t FONT_SIZE;
  static const spec_t FONT;
  static const spec_t USE_BASELINE;
  virtual void setStyle(prop_t const& prop) override;
  virtual prop_t getStyle(spec_t spec) const override;
  virtual style_const_t getDefaultStyle() const override;

  void setText(std::string text);
  std::string getText() const;

  void setColor(glm::vec4 color);
  glm::vec4 getColor() const;

  void setFontSize(float fontSize);
  float getFontSize() const;

  void setFont(std::shared_ptr<const Font> font);
  std::shared_ptr<const Font> getFont() const;

  void setUseBaseline(bool useBaseline);
  bool getUseBaseline() const;

private:
  void computeSize();
  std::string text;
  glm::vec4 color;
  float fontSize;
  std::shared_ptr<const Font> font;
  Shader* shader;
  bool useBaseline;
  float baselineOffset;
};

}; // namespace ui
