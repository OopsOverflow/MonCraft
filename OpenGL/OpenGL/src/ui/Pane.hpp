#pragma once

#include "Component.hpp"
#include "gl/Shader.hpp"

namespace ui {

class Pane : public Component {

public:
  Pane(Component* parent);
  virtual ~Pane();

  void draw() override;

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
