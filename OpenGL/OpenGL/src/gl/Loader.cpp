#include "Loader.hpp"

#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdexcept>
#include "debug/Debug.hpp"

Loader::~Loader()
{
    ASSERT_GL_MAIN_THREAD();
    for (auto& i : this->textureList)
        glDeleteTextures(1, &i);
}

GLuint Loader::loadTexture(const std::string& fileName) {
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
      throw std::runtime_error("could not load SDL2_image with PNG files");
    }

    std::string path = "data/img/" + fileName + ".png";
    SDL_Surface* img = IMG_Load(path.c_str());

    if(!img) {
      throw std::runtime_error("image file not found: " + path);
    }

    SDL_Surface* rgbImg = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(img);

    std::vector<SDL_Surface*> mipmaps;
    for(int i = 1; img; i++) {
      std::string path = "data/img/" + fileName + std::to_string(i) + ".png";
      img = IMG_Load(path.c_str());
      if(img) {
        SDL_Surface* surf = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(img);
        if(surf) mipmaps.push_back(surf);
      }
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    textureList.push_back(textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbImg->w, rgbImg->h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)rgbImg->pixels);
        SDL_FreeSurface(rgbImg); //Delete the surface at the end of the program

        if(mipmaps.empty()) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmaps.size());
            for(size_t i = 0; i < mipmaps.size(); i++) {
                SDL_Surface* surf = mipmaps.at(i);
                glTexImage2D(GL_TEXTURE_2D, i+1, GL_RGBA, surf->w, surf->h, 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)surf->pixels);
                SDL_FreeSurface(surf);
            }
        }
    }

    return textureID;
}

GLuint Loader::loadCubeMap(std::vector<std::string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::string path = "data/img/" + faces[i];
        SDL_Surface* img = IMG_Load(path.c_str());
        SDL_Surface* rgbImg = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA32, 0);
        if (rgbImg)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, rgbImg->w, rgbImg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)rgbImg->pixels);
            SDL_FreeSurface(rgbImg);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            SDL_FreeSurface(rgbImg);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
