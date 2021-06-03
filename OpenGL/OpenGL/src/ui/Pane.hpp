#pragma once

#include "Component.hpp"
#include "gl/Shader.hpp"

namespace ui {

class Pane : public Component {

public:
  Pane(Component* parent);
  virtual ~Pane();

  void draw() override;

  static const spec_t COLOR;
  virtual void setStyle(prop_t prop) override;

  void setColor(glm::vec4 color);
  glm::vec4 getColor() const;


private:
  void computeModel();
  Shader* shader;

  glm::vec4 color;

  GLuint vao, vbo;
  glm::mat4 model;
};

}; // namespace ui
