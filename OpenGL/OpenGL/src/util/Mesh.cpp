#include "Mesh.hpp"

#include <iostream>

Mesh::Mesh(GLuint vao, GLuint vbo, GLuint ebo, GLuint vertCount)
    : myVAO(vao), myVBO(vbo), myEBO(ebo), myVertCount(vertCount)
{ }

Mesh::Mesh(std::vector<GLfloat> const& positions,
     std::vector<GLfloat> const& normals,
     std::vector<GLfloat> const& textureCoords,
     std::vector<GLuint>  const& indices)
{
  myVertCount = (GLuint)indices.size();

  glGenVertexArrays(1, &myVAO);
  glGenBuffers(1, &myVBO);
  glGenBuffers(1, &myEBO);

  glBindVertexArray(myVAO);
  glBindBuffer(GL_ARRAY_BUFFER, myVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEBO);

  size_t size = positions.size() / 3 * sizeof(GLfloat);

  glBufferData(GL_ARRAY_BUFFER, size * 8, nullptr, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, myVertCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  // positions
  glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3, positions.data());
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
  glEnableVertexAttribArray(0);

  // normals
  glBufferSubData(GL_ARRAY_BUFFER, size * 3, size * 3, normals.data());
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(size * 3));
  glEnableVertexAttribArray(1);

  // textureCoords
  glBufferSubData(GL_ARRAY_BUFFER, size * 6, size * 2, textureCoords.data());
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(size * 6));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
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
