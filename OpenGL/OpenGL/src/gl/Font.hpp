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
  void draw(std::u32string text, glm::vec3 pos, float scale, glm::vec4 color) const;

  struct Glyph {
    GLuint tex;
    glm::ivec2 size;
    glm::ivec2 bearing;
    FT_Long advance;
  };

  Glyph const& getChar(FT_ULong charcode) const;

private:

  std::vector<Glyph> glyphs;
  FT_Library ft;
  FT_Face face;

  GLuint vao, vbo;

  void loadAllGlyphs();
  Glyph loadGlyph(FT_UInt i) const;
  void drawGlyph(GLuint tex, glm::vec3 pos, glm::vec2 size) const;
};
