#include "mesh.hpp"

#include <iostream>

mesh::mesh(GLuint VAO, GLuint count, GLuint vert, GLuint tex)
    : myVAO(VAO)
    , myCount(count)
    , myVert(vert)
    , myTex(tex)
{ }

GLuint mesh::getVAO() const
{
    return myVAO;
}

GLuint mesh::getVertexCount() const
{
    return myCount;
}


mesh :: ~mesh()
{
    glDeleteBuffers(1, &myVert);
    glDeleteBuffers(1, &myTex);
    glDeleteVertexArrays(1, &myVAO);
}