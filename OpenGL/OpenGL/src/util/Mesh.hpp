#ifndef Mesh_H
#define Mesh_H

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <vector>

class Mesh
{
public:
    Mesh(std::vector<GLfloat> const& positions,
         std::vector<GLfloat> const& normals,
         std::vector<GLfloat> const& textureCoords,
         std::vector<GLuint>  const& indices);

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
