#include "Mesh.hpp"

#include <iostream>

Mesh::Mesh(GLuint vao, GLuint vbo, GLuint vertCount)
    : myVAO(vao), myVBO(vbo), myVertCount(vertCount)
{ }

{
}

GLuint Mesh::getVAO() const {
  return myVAO;
}

GLuint Mesh::getVertexCount() const {
  return myVertCount;
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &myVAO);
  glDeleteBuffers(1, &myVBO);
  glDeleteBuffers(1, &myEBO);
}
