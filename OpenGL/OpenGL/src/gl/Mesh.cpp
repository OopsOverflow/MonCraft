#include "Mesh.hpp"

#include <iostream>
#include "gl/Shader.hpp"

Mesh::Mesh(GLuint vao, GLuint vbo, GLuint ebo, GLuint vertCount)
    : myVAO(vao), myVBO(vbo), myEBO(ebo), myVertCount(vertCount)
{ }

Mesh::Mesh(std::vector<GLfloat> const& positions,
     std::vector<GLfloat> const& normals,
     std::vector<GLfloat> const& textureCoords,
     std::vector<GLfloat> const& occlusion,
     std::vector<GLuint>  const& indices,
     std::vector<GLfloat> const& normalMapCoords)
{
  myVertCount = (GLuint)indices.size();

  glGenVertexArrays(1, &myVAO);
  glGenBuffers(1, &myVBO);
  glGenBuffers(1, &myEBO);

  glBindVertexArray(myVAO);
  glBindBuffer(GL_ARRAY_BUFFER, myVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myEBO);

  size_t size = positions.size() / 3 * sizeof(GLfloat);

  glBufferData(GL_ARRAY_BUFFER, size * 11, nullptr, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, myVertCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

  // positions
  glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3, positions.data());
  glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
  glEnableVertexAttribArray(VERTEX_POSITION);

  // normals
  glBufferSubData(GL_ARRAY_BUFFER, size * 3, size * 3, normals.data());
  glVertexAttribPointer(VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(size * 3));
  glEnableVertexAttribArray(VERTEX_NORMAL);

  // textureCoords
  glBufferSubData(GL_ARRAY_BUFFER, size * 6, size * 2, textureCoords.data());
  glVertexAttribPointer(VERTEX_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(size * 6));
  glEnableVertexAttribArray(VERTEX_TEXTURE);

  // occlusion
  glBufferSubData(GL_ARRAY_BUFFER, size * 8, size * 1, occlusion.data());
  glVertexAttribPointer(VERTEX_OCCLUSION, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(size * 8));
  glEnableVertexAttribArray(VERTEX_OCCLUSION);

  // normalMapCoords
  glBufferSubData(GL_ARRAY_BUFFER, size * 9, size * 2, normalMapCoords.data());
  glVertexAttribPointer(VERTEX_NORMALTEXTURE, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(size * 9));
  glEnableVertexAttribArray(VERTEX_NORMALTEXTURE);

  glBindVertexArray(0);
}

Mesh::Mesh(MeshData const& data)
 : Mesh(data.positions, data.normals, data.textureCoords, data.occlusion, data.indices, data.normalMapCoords)
{ }

Mesh::Mesh(Mesh&& other) {
  myVAO = other.myVAO;
  myVBO = other.myVBO;
  myEBO = other.myEBO;
  myVertCount = other.myVertCount;
  other.myVAO = 0;
  other.myVBO = 0;
  other.myEBO = 0;
  other.myVertCount = 0;
}

GLuint Mesh::getVAO() const {
  return myVAO;
}

GLuint Mesh::getVertexCount() const {
  return myVertCount;
}

#include "debug/Debug.hpp"

Mesh::~Mesh() {
  ASSERT_GL_MAIN_THREAD();
  glDeleteVertexArrays(1, &myVAO);
  glDeleteBuffers(1, &myVBO);
  glDeleteBuffers(1, &myEBO);
}
