#include "Mesh.hpp"

#include <iostream>
#include <algorithm>

#include "gl/Shader.hpp"
#include "debug/Debug.hpp"

Mesh::Mesh(GLuint vao, GLuint vbo, GLuint ebo, GLsizei vertCount) noexcept
    : vao(vao), vbo(vbo), ebo(ebo), vertCount(vertCount)
{ }

Mesh::Mesh(std::vector<GLfloat> const& positions,
     std::vector<GLfloat> const& normals,
     std::vector<GLfloat> const& textureCoords,
     std::vector<GLfloat> const& occlusion,
     std::vector<GLuint>  const& indices,
     std::vector<GLfloat> const& normalMapCoords)
{
  ASSERT_GL_MAIN_THREAD();

  vertCount = (GLsizei)indices.size();

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  size_t size = positions.size() / 3 * sizeof(GLfloat);

  glBufferData(GL_ARRAY_BUFFER, size * 11, nullptr, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

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

static void reverseTriangles(std::vector<GLuint>& vec) {
  if(vec.size() % 3 != 0) throw std::runtime_error("reverseQuads: size is not a multiple of 3");
  if(vec.size() < 6) return;

  auto first = vec.begin();
  auto last = vec.end();
  std::advance(last, -3);

  while (first < last) {
    std::iter_swap(first++, last++);
    std::iter_swap(first++, last++);
    std::iter_swap(first++, last++);
    std::advance(last, -6);
  }
}

Mesh::Mesh(MeshData& data) noexcept
{
  ASSERT_GL_MAIN_THREAD();
  size_t transpCount = data.indicesTranspX.size() + data.indicesTranspY.size() + data.indicesTranspZ.size();
  vertCount = (GLsizei)(data.indicesSolid.size() + transpCount);

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  size_t size = data.positions.size() / 3 * sizeof(GLfloat);

  glBufferData(GL_ARRAY_BUFFER, size * 11, nullptr, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (data.indicesSolid.size() + 2 * transpCount) * sizeof(GLuint), nullptr, GL_STATIC_DRAW);
  size_t indOff = 0;
  if(data.indicesSolid.size() != 0)
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indOff, data.indicesSolid.size() * sizeof(GLuint), data.indicesSolid.data());
  indOff += data.indicesSolid.size() * sizeof(GLuint);

  // indices in positive x/y/z direction
  if(data.indicesTranspX.size() != 0)
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indOff, data.indicesTranspX.size() * sizeof(GLuint), data.indicesTranspX.data());
  indOff += data.indicesTranspX.size() * sizeof(GLuint);
  if(data.indicesTranspY.size() != 0)
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indOff, data.indicesTranspY.size() * sizeof(GLuint), data.indicesTranspY.data());
  indOff += data.indicesTranspY.size() * sizeof(GLuint);
  if(data.indicesTranspZ.size() != 0)
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indOff, data.indicesTranspZ.size() * sizeof(GLuint), data.indicesTranspZ.data());
  indOff += data.indicesTranspZ.size() * sizeof(GLuint);

  // indices in negative x/y/z direction
  reverseTriangles(data.indicesTranspX);
  reverseTriangles(data.indicesTranspY);
  reverseTriangles(data.indicesTranspZ);
  if(data.indicesTranspX.size() != 0)
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indOff, data.indicesTranspX.size() * sizeof(GLuint), data.indicesTranspX.data());
  indOff += data.indicesTranspX.size() * sizeof(GLuint);
  if(data.indicesTranspY.size() != 0)
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indOff, data.indicesTranspY.size() * sizeof(GLuint), data.indicesTranspY.data());
  indOff += data.indicesTranspY.size() * sizeof(GLuint);
  if(data.indicesTranspZ.size() != 0)
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indOff, data.indicesTranspZ.size() * sizeof(GLuint), data.indicesTranspZ.data());

  // positions
  glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3, data.positions.data());
  glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
  glEnableVertexAttribArray(VERTEX_POSITION);

  // normals
  glBufferSubData(GL_ARRAY_BUFFER, size * 3, size * 3, data.normals.data());
  glVertexAttribPointer(VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(size * 3));
  glEnableVertexAttribArray(VERTEX_NORMAL);

  // textureCoords
  glBufferSubData(GL_ARRAY_BUFFER, size * 6, size * 2, data.textureCoords.data());
  glVertexAttribPointer(VERTEX_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(size * 6));
  glEnableVertexAttribArray(VERTEX_TEXTURE);

  // occlusion
  glBufferSubData(GL_ARRAY_BUFFER, size * 8, size * 1, data.occlusion.data());
  glVertexAttribPointer(VERTEX_OCCLUSION, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(size * 8)); // TODO: smaller type
  glEnableVertexAttribArray(VERTEX_OCCLUSION);

  // normalMapCoords
  glBufferSubData(GL_ARRAY_BUFFER, size * 9, size * 2, data.normalMapCoords.data());
  glVertexAttribPointer(VERTEX_NORMALTEXTURE, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(size * 9));
  glEnableVertexAttribArray(VERTEX_NORMALTEXTURE);

  glBindVertexArray(0);
}

Mesh::Mesh(Mesh&& other) noexcept {
  vao = other.vao;
  vbo = other.vbo;
  ebo = other.ebo;
  vertCount = other.vertCount;
  other.vao = 0;
  other.vbo = 0;
  other.ebo = 0;
  other.vertCount = 0;
}

GLuint Mesh::getVAO() const {
  return vao;
}

GLsizei Mesh::getVertexCount() const {
  return vertCount;
}

Mesh::~Mesh() {
  ASSERT_GL_MAIN_THREAD();
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}
