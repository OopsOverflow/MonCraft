#include "ResourceManager.hpp"
#include <debug/Debug.hpp>

std::unordered_map<std::string, std::unique_ptr<Shader>> ResourceManager::shaders;
std::unordered_map<std::string, GLuint> ResourceManager::textures;

ResourceManager::ResourceManager() {

}

void ResourceManager::addShader(std::string const& name, std::unique_ptr<Shader> shader) {
  auto res = shaders.emplace(name, std::move(shader));
  if(!res.second) {
    std::string err = "shader already inserted: " + name;
    throw std::runtime_error(err);
  }
}

void ResourceManager::addShader(std::string const& name, std::string const& vert, std::string const& frag) {
  std::unique_ptr<Shader> shader(new Shader(vert.c_str(), frag.c_str()));
  addShader(name, std::move(shader));
}

Shader* ResourceManager::getShader(std::string const& name) {
  auto it = shaders.find(name);
  if(it == shaders.end()) {
    std::string err = "shader not found: " + name;
    throw std::runtime_error(err);
  }
  return it->second.get();
}


void ResourceManager::free() {
  shaders.clear();

  for(auto pair : textures)
    glDeleteTextures(1, &pair.second);
  textures.clear();
}
