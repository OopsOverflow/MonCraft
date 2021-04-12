#pragma once
#ifndef mesh_H
#define mesh_H

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh
{
public:
    Mesh() = default;
    Mesh(GLuint vao, GLuint count, GLuint vert, GLuint tex);

    GLuint getVAO() const;
    GLuint getVertexCount() const;

    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 sca = { 1, 1, 1 };

    ~Mesh();

    GLuint myCount;

private:
    GLuint myVAO;

    GLuint myVert;
    GLuint myTex;
};

#endif // mesh_H
