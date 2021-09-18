#include "ResourceManager.hpp"
#include <debug/Debug.hpp>
#include <SDL2/SDL_image.h>

std::unordered_map<std::string, std::unique_ptr<Shader>> ResourceManager::shaders;
std::unordered_map<std::string, GLuint> ResourceManager::textures;

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
  free();
}

void ResourceManager::init() {
  if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    throw std::runtime_error("could not load SDL2_image with PNG files");
  }
}

void ResourceManager::free() {
  std::cout << "freeing resources..." << std::endl;
  shaders.clear();

  for(auto pair : textures)
    glDeleteTextures(1, &pair.second);
  textures.clear();
}

//// shader stuff ////

Shader* ResourceManager::addShader(std::string const& name, std::unique_ptr<Shader> shader) {
  auto res = shaders.emplace(name, std::move(shader));
  if(!res.second) {
    throw std::runtime_error("shader already inserted: " + name);
  }
  return res.first->second.get();
}

Shader* ResourceManager::loadShader(std::string const& name, std::string const& vert, std::string const& frag) {
  std::cout << "loading shader: " << name << std::endl;
  std::unique_ptr<Shader> shader(new Shader(("src/shader/" + vert).c_str(), ("src/shader/" + frag).c_str()));
  return addShader(name, std::move(shader));
}

Shader* ResourceManager::getShader(std::string const& name) {
  auto it = shaders.find(name);
  if(it == shaders.end()) {
    std::string err = "shader not found: " + name;
    throw std::runtime_error(err);
  }
  return it->second.get();
}

//// texture stuff ////

GLuint ResourceManager::addTexture(std::string const& name, GLuint texture) {
  auto res = textures.emplace(name, texture);
  if(!res.second) {
    throw std::runtime_error("texture already inserted: " + name);
  }
  return res.first->second;
}

GLuint ResourceManager::loadTexture(std::string const& name, std::string const& filename) {
    std::string path = "data/img/" + filename + ".png";
    SDL_Surface* img = IMG_Load(path.c_str());

    if(!img) {
      throw std::runtime_error("image file not found: " + path);
    }

    SDL_Surface* rgbImg = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(img);

    std::vector<SDL_Surface*> mipmaps;
    for(int i = 1; img; i++) {
      std::string path = "data/img/" + filename + std::to_string(i) + ".png";
      img = IMG_Load(path.c_str());
      if(img) {
        SDL_Surface* surf = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(img);
        if(surf) mipmaps.push_back(surf);
      }
    }

    // create the texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    addTexture(name, textureID);

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
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            float borderColor[] = { 0.5f, 0.5f, 1.0f, 0.0f };//generation for normal map (straight bounce if out of border)
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

GLuint ResourceManager::loadCubeMap(std::string const& name, std::vector<std::string> const& faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++) {
        std::string path = "data/img/" + faces.at(i);
        SDL_Surface* img = IMG_Load(path.c_str());
        SDL_Surface* rgbImg = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGBA32, 0);
        if (rgbImg) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, rgbImg->w, rgbImg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)rgbImg->pixels);
            SDL_FreeSurface(img);
            SDL_FreeSurface(rgbImg);
        }
        else {
            SDL_FreeSurface(img);
            SDL_FreeSurface(rgbImg);
            throw std::runtime_error("Cubemap tex failed to load at path: " + faces[i]);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    addTexture(name, textureID);
    return textureID;
}

GLuint ResourceManager::getTexture(std::string const& name) {
  auto it = textures.find(name);
  if(it == textures.end()) {
    std::string err = "texture not found: " + name;
    throw std::runtime_error(err);
  }
  return it->second;
}
