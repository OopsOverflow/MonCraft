#include "Loader.hpp"

#include <SDL2/SDL_image.h>
#include <iostream>

Loader::~Loader()
{
    for (auto& i : this->textureList)
        glDeleteTextures(1, &i);
}


//The meshes have to be pointers, or else if I return a copy, the destructor is called on the local copy and thus OpenGL
//deletes the vertex arrays (from the destructor)
ptr2mesh Loader::loadToVAO(const std::vector<GLfloat>& positions,
    const std::vector<GLuint>& indices,
    const std::vector<GLfloat>& texture)
{
    GLuint id = createVAO();
    bindIndexBuffer(indices);

    GLuint vert = storeDataInAttributeList(0, 3, positions);
    GLuint txr = storeDataInAttributeList(1, 2, texture);
    unbindVAO();

    return std::make_unique<Mesh>(id, indices.size(), vert, txr);
}

ptr2mesh Loader::loadToVAO(const std::vector<GLfloat>& positions,
    const std::vector<GLfloat>& texture)
{
    GLuint id = createVAO();

    GLuint vert = storeDataInAttributeList(0, 3, positions);
    GLuint txr = storeDataInAttributeList(1, 2, texture);
    unbindVAO();

    return std::make_unique<Mesh>(id, positions.size() / 3, vert, txr);
}

GLuint Loader::loadTexture(const std::string& fileName)
{
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << "Could not load SDL2_image with PNG files\n";
        return EXIT_FAILURE;
    }

    // Load the file(png)
    std::string path = "data/img/" + fileName + ".png";
    SDL_Surface * img = IMG_Load(path.c_str());
    //Convert to an RGBA8888 surface
    SDL_Surface* rgbImg = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA32, 0);
    //Delete the old surface
    SDL_FreeSurface(img);

    GLuint textureID;
    glGenTextures(1, &textureID);
    textureList.push_back(textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbImg->w, rgbImg->h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)rgbImg->pixels);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    SDL_FreeSurface(rgbImg); //Delete the surface at the end of the program

    return textureID;
}



void Loader::bindIndexBuffer(const std::vector<GLuint>& indices)
{
    createVBO(GL_ELEMENT_ARRAY_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.at(0)), indices.data(), GL_STATIC_DRAW);
}


//Vertex ARRAY object
GLuint Loader::createVAO()
{
    GLuint id;
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
    return id;
}

//Vertex BUFFER object
GLuint Loader::createVBO(GLenum type)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(type, vbo);
    return vbo;
}

void Loader::unbindVAO()
{
    glBindVertexArray(0);
}
