#ifndef Loader_H
#define Loader_H

#include <vector>
#include <memory>
#include <string>

#include "GL/glew.h"
#include "Mesh.hpp"

typedef std::unique_ptr<Mesh> ptr2mesh;


class Loader
{
public:
    ~Loader();

    ptr2mesh loadToVAO(const std::vector<GLfloat>& positions,
        const std::vector<GLuint>& indices,
        const std::vector<GLfloat>& texture);

    ptr2mesh loadToVAO(const std::vector<GLfloat>& positions,
        const std::vector<GLfloat>& texture);

    GLuint loadTexture(const std::string& fileName);


private:
    void bindIndexBuffer(const std::vector<GLuint>& indices);

    GLuint createVAO();
    GLuint createVBO(GLenum type);
    void unbindVAO();

    template<typename T>
    GLuint storeDataInAttributeList(int attributeID, int coordCount, const std::vector<T>& data)
    {
        GLuint vbo = createVBO(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data.at(0)), data.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(attributeID, coordCount, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(attributeID);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return vbo;

    }

    std::vector<GLuint> vaoList;
    std::vector<GLuint> vboList;
    std::vector<GLuint> textureList;
};

#endif // Loader_H
