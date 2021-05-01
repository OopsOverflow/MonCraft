#include "Member.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../gl/Shader.hpp"

using glm::mat4;
using glm::vec2;

Member::Member(Mesh mesh)
  : mesh(std::move(mesh))
{}

Member::~Member() {}

void Member::draw() {
  glm::mat4 modelMatrix = node.model * geometryModel;
  glBindVertexArray(mesh.getVAO());
  glUniformMatrix4fv(MATRIX_MODEL, 1, GL_FALSE, glm::value_ptr(modelMatrix));
  glDrawElements(GL_TRIANGLES, mesh.getVertexCount(), GL_UNSIGNED_INT, nullptr);
}
