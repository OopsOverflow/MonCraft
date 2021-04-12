#ifndef Mesh_H
#define Mesh_H

#include "GL/glew.h"
#include <glm/glm.hpp>

class Mesh
{
public:
    Mesh(GLuint vao, GLuint count, GLuint vert, GLuint tex);
    Mesh(GLuint vao, GLuint vbo, GLuint vertCount);
    ~Mesh();

    GLuint getVAO() const;
    GLuint getVertexCount() const;

    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 sca = { 1, 1, 1 };

private:
    GLuint myVAO;
    GLuint myVBO;
    GLuint myEBO;
    GLuint myVertCount;
};

#endif // Mesh_H
