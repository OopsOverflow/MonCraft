#include "Root.hpp"
#include "gl/Shader.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "gl/ResourceManager.hpp"

using namespace ui;
using namespace glm;

Root::Root(ivec2 size)
  : Component(nullptr),
    size(size)
{}


void Root::setSize(glm::ivec2 size) {
  this->size = size;
}

glm::ivec2 Root::getSize() const {
  return size;
  shaders = {
    ResourceManager::getShader("font"),
    ResourceManager::getShader("pane"),
  };
}

void Root::render() {
  auto proj = glm::ortho(0.0f, (float)size.x, 0.0f, (float)size.y);
  auto I = glm::mat4(1.f);

  for(Shader* shader: shaders) {
    shader->activate();
    glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(I));
    glUniformMatrix4fv(MATRIX_VIEW, 1, GL_FALSE, glm::value_ptr(I));
    glUniformMatrix4fv(MATRIX_PROJECTION, 1, GL_FALSE, glm::value_ptr(proj));
  }
  draw();
  queueDraw(); // TODO: for now we queueDraw everything at each frame
}
