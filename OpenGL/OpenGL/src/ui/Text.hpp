#pragma once

#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "ui/Component.hpp"
#include "ui/style/Property.hpp"
#include "ui/style/Specification.hpp"
#include "ui/style/Style.hpp"

class Font;
class Shader;

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
  std::string text;
  Shader* shader;
  float baselineOffset;
};

}; // namespace ui
