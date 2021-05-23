#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "Component.hpp"
#include "gl/Font.hpp"

namespace ui {

class Text : public Component {

public:
  Text(Component* parent, std::string text, std::shared_ptr<const Font> font);

  void draw() override;

  glm::ivec2 getSize() const override;

  void setText(std::string text);
  std::string getText() const;

  void setColor(glm::vec4 color);
  glm::vec4 getColor() const;

  void setFontSize(float fontSize);
  float getFontSize() const;

  void setFont(std::shared_ptr<const Font> font);
  std::shared_ptr<const Font> getFont() const;

private:
  std::string text;
  glm::vec4 color;
  float fontSize;
  std::shared_ptr<const Font> font;
  Shader* shader;
};

}; // namespace ui
