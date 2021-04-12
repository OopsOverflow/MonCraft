#include "Mesh.hpp"

#include <iostream>

Mesh::Mesh(GLuint VAO, GLuint count, GLuint vert, GLuint tex)
    : myVAO(VAO)
    , myCount(count)
    , myVert(vert)
    , myTex(tex)
{ }

GLuint Mesh::getVAO() const
{
    return myVAO;
}

GLuint Mesh::getVertexCount() const
{
    return myCount;
}


Mesh::~Mesh()
{
    glDeleteBuffers(1, &myVert);
    glDeleteBuffers(1, &myTex);
    glDeleteVertexArrays(1, &myVAO);
}
