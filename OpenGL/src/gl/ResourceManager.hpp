#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

class Font;
class Shader;

class ResourceManager {

public:
  static Shader* addShader(std::string const& name, std::unique_ptr<Shader> shader);
  static Shader* loadShader(std::string const& name, std::string const& vert, std::string const& frag);
  static Shader* getShader(std::string const& name);

  static GLuint addTexture(std::string const& name, GLuint texture);
  static GLuint loadTexture(std::string const& name, std::string const& filename);
  static GLuint loadCubeMap(std::string const& name, std::vector<std::string> const& faces);
  static GLuint getTexture(std::string const& name);

  static void loadFont(std::string const& name, std::string const& filename);
  static std::shared_ptr<const Font> getFont(std::string const& name);

  static void free();

private:
  static void init();
  ResourceManager();
  ~ResourceManager();
  static std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
  static std::unordered_map<std::string, GLuint> textures;
  static std::unordered_map<std::string, std::shared_ptr<const Font> > fonts;
};
