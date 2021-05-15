#ifndef Mesh_H
#define Mesh_H

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <vector>

struct MeshData {
  std::vector<GLuint> scheme;
  std::vector<GLuint> indicesSolid, indicesTranspX, indicesTranspY, indicesTranspZ;
  std::vector<GLfloat> positions, normals, textureCoords, occlusion, normalMapCoords;
};

/**
 * A mesh holds a vao / vbo / ebo and manages their lifetimes.
 */

class Mesh
{
public:

    /**
     * Creates using the given vectors of vertex data.
     */
    Mesh(std::vector<GLfloat> const& positions,
        std::vector<GLfloat> const& normals,
        std::vector<GLfloat> const& textureCoords,
        std::vector<GLfloat> const& occlusion,
        std::vector<GLuint>  const& indices,
        std::vector<GLfloat> const& normalMapCoords);

    /**
     * Creates using the given vectors of vertex data.
     */
    Mesh(MeshData const& data);


    /**
     * Creates a mesh from a vao / vbo / ebo and vertex count.
     * This mesh becomes owner of those and manages their lifetimes.
     * Thus, the vao/vbo/ebo must be owner-free before assigning them to
     * the mesh.
     */
    Mesh(GLuint vao, GLuint vbo, GLuint ebo, GLuint vertCount);
    ~Mesh();

    Mesh(Mesh const&) = delete;
    Mesh& operator=(Mesh const&) = delete;

    Mesh(Mesh&&);

    /**
     * Gets the vao.
     */
    GLuint getVAO() const;

    /**
     * Gets the vertex count.
     */
    GLuint getVertexCount() const;

    /**
     * The model member of the mesh is read/write and can be modified by anyone.
     */
    glm::mat4 model = glm::mat4(1.f);

private:
    GLuint myVAO;
    GLuint myVBO;
    GLuint myEBO;
    GLuint myVertCount;
};

#endif // Mesh_H
