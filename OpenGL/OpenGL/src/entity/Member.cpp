#include "Member.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../gl/Shader.hpp"
#include "debug/Debug.hpp"
#include "gl/SafeMesh.hpp"

using glm::mat4;
using glm::vec2;

Member::Member()
  : geometryModel(1.f), mesh(nullptr)
{}

Member::~Member() {
  SafeMesh(std::move(mesh));
}

void Member::draw() {
  if(!mesh) {
    ASSERT_GL_MAIN_THREAD();
    mesh = createMesh();
  }
  mat4 modelMatrix = mat4(node.model) * geometryModel;
  glBindVertexArray(mesh->getVAO());
  glUniformMatrix4fv(Shader::getActive()->getUniform(MATRIX_MODEL), 1, GL_FALSE, glm::value_ptr(modelMatrix));
  glDrawElements(GL_TRIANGLES, mesh->getVertexCount(), GL_UNSIGNED_INT, nullptr);
}
