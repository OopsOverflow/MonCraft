#include "Pane.hpp"
#include "debug/Debug.hpp"
#include "gl/ResourceManager.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ui;
using namespace glm;

const spec_t Pane::COLOR = MAKE_SPEC("Pane::color", vec4);

static const GLfloat quad[6][2] = {
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },

    { 1.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
};

Pane::Pane(Component* parent)
  : Component(parent),
    color(1.f)
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 2, quad, GL_STATIC_DRAW);
  glVertexAttribPointer(VERTEX_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(VERTEX_POSITION);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  shader = ResourceManager::getShader("pane");

  Pane::getDefaultStyle()->apply(this);
}

Pane::~Pane() {
  ASSERT_GL_MAIN_THREAD();
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void Pane::setStyle(prop_t const& prop) {
  if(prop.spec == Pane::COLOR) {
    setColor(prop.value->get<vec4>());
  }
  else {
    Component::setStyle(prop);
  }
}

prop_t Pane::getStyle(spec_t spec) const {
  if(spec == Pane::COLOR) {
    return make_property(spec, getColor());
  }
  else {
    return Component::getStyle(spec);
  }
}

style_const_t Pane::getDefaultStyle() const {
  static style_const_t style = Style::make_style(
    Component::getDefaultStyle(),
    make_property(Pane::COLOR, vec4(1.f))
  );

  return style;
}

void Pane::draw() {
  computeModel();
  glBindVertexArray(vao);
  shader->activate();
  glUniform4fv(shader->getUniformLocation("color"), 1, glm::value_ptr(color));
  glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
  Component::draw();
}

void Pane::computeModel() {
  auto p1 = getAbsoluteOrigin();
  model = mat4(1.f);
  model = translate(model, vec3(p1, 0.f));
  model = scale(model, vec3(getAbsoluteSize(), 1.f));
}

void Pane::setColor(glm::vec4 color) {
  if(color == this->color) return;
  this->color = color;
  queueDraw();
}

glm::vec4 Pane::getColor() const {
  return color;
}
