#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <codecvt>

#include "Component.hpp"
#include "gl/Font.hpp"

namespace ui {

class Text : public Component {

protected:
  Text(std::string text, std::shared_ptr<const Font> font);

public:
  static std::unique_ptr<Text> create(std::string text, std::shared_ptr<const Font> font);

  void draw() override;

  static const spec_t COLOR;
  static const spec_t FONT_SIZE;
  static const spec_t FONT;
  static const spec_t USE_BASELINE;
  
  virtual void setProperty(prop_t prop) override;
  virtual prop_t getProperty(spec_t spec) const override;
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
  std::u32string text;
  Shader* shader;
  float baselineOffset;
  static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
};

}; // namespace ui
