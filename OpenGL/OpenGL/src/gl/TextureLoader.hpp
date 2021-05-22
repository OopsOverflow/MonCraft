#pragma once

#include <vector>
#include <memory>
#include <string>

#include "GL/glew.h"
#include "Mesh.hpp"

typedef std::unique_ptr<Mesh> ptr2mesh;

/**
 * A texture loader.
 */

class TextureLoader
{
public:
    ~TextureLoader();

    /**
     * Loads the texture with the given filename (must be a .png file and
     * filename must NOT include the file extension).
     * The generated texture will load the custom mipmaps if they exist, or
     * generate them by default.
     * (files <filename>1.png -- <filename>N.png are the N mipmap levels)
     */
    GLuint loadTexture(const std::string& filename);

    /**
     * Loads the cube map texture. faces holds a list of the filenames for the
     * cubemap.
     */
    GLuint loadCubeMap(const std::vector<std::string>& faces);

private:
    void bindIndexBuffer(const std::vector<GLuint>& indices);

    GLuint createVAO();
    GLuint createVBO(GLenum type);
    void unbindVAO();

    template<typename T>
    GLuint storeDataInAttributeList(int attributeID, int coordCount, const std::vector<T>& data)
    {
        GLuint vbo = createVBO(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(attributeID, coordCount, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(attributeID);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return vbo;
    }

    std::vector<GLuint> textureList;
};
