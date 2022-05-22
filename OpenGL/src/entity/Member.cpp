#include "Member.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "debug/Debug.hpp"
#include "entity/Node.hpp"
#include "gl/Mesh.hpp"
#include "gl/SafeMesh.hpp"
#include "gl/Shader.hpp"

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


void Member::animate(float dt) {
  node.lookAt(anim->computeAnim(dt));
}