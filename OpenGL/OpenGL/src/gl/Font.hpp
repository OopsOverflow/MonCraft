#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <map>
#include <glm/glm.hpp>
#include "gl/Shader.hpp"
#include "gl/Mesh.hpp"

// thanks https://learnopengl.com/In-Practice/Text-Rendering
class Font {

public:
  Font(std::string const& filename);
  virtual ~Font();

  void draw(std::string text, glm::vec3 pos, float scale, glm::vec4 color) const;

  struct Character {
    GLuint tex;
    glm::ivec2 size;
    glm::ivec2 bearing;
    FT_Long advance;
  };
  std::map<char, Character> characters;

private:
  FT_Library ft;
  FT_Face face;

  GLuint vao, vbo;

  void loadAllGlyphs();
};
