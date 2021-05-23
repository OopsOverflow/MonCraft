#pragma once

#include "Shader.hpp"

#include <unordered_map>
#include <memory>

class ResourceManager {

public:
  static void addShader(std::string const& name, std::unique_ptr<Shader> shader);
  static void addShader(std::string const& name, std::string const& vert, std::string const& frag);
  static Shader* getShader(std::string const& name);

  static void addTexture(GLuint texture, std::string const& name);
  static GLuint getTexture(std::string const& name);

  void free();

private:
  ResourceManager();
  static std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
  static std::unordered_map<std::string, GLuint> textures;
};
