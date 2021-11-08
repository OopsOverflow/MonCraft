#pragma once

#include <GL/glew.h>
#include <memory>
#include <glm/glm.hpp>

#include "ui/Component.hpp"
#include "ui/style/Property.hpp"
#include "ui/style/Specification.hpp"
#include "ui/style/Style.hpp"

class Shader;

namespace ui {

class Pane : public Component {

protected:
  Pane();

public:
  static std::unique_ptr<Pane> create();
  virtual ~Pane();

  void draw() override;

  static const spec_t COLOR;

  virtual void setProperty(prop_t prop) override;
  virtual prop_t getProperty(spec_t spec) const override;
  virtual style_const_t getDefaultStyle() const override;

  void setColor(glm::vec4 color);
  glm::vec4 getColor() const;


private:
  void computeModel();
  Shader* shader;

  GLuint vao, vbo;
  glm::mat4 model;
};

}; // namespace ui
