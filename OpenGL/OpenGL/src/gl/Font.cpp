#include "Font.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>

#include "debug/Debug.hpp"
#include "gl/Shader.hpp"
#include "gl/UTF8Iter.hpp"

using namespace glm;

Font::Font(std::string const& filename) {
  if (FT_Init_FreeType(&ft)) {
    throw std::runtime_error("failed to initialize Freetype");
  }

  std::string path = "data/fonts/" + filename + ".ttf";
  if (FT_New_Face(ft, path.c_str(), 0, &face)) {
    throw std::runtime_error("failed to load Freetype font");
  }

  FT_Set_Pixel_Sizes(face, 0, 48);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  loadAllGlyphs();

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, nullptr, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(VERTEX_POSITION);
  glVertexAttribPointer(VERTEX_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(3 * 6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(VERTEX_TEXTURE);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Font::~Font() {
  ASSERT_GL_MAIN_THREAD();
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

Font::Glyph Font::loadGlyph(FT_UInt i) const {
  if (FT_Load_Glyph(face, i, FT_LOAD_RENDER))   {
    std::cout << "[Warning] Failed to load Glyph index " << i << std::endl;
    FT_Load_Char(face, 0, FT_LOAD_RENDER); // 0 is missing glyph
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_R8,
    face->glyph->bitmap.width,
    face->glyph->bitmap.rows,
    0, GL_RED, GL_UNSIGNED_BYTE,
    face->glyph->bitmap.buffer
  );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  return Glyph{
    texture,
    ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
    ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
    face->glyph->advance.x >> 6 // advance is in 1/64th pixels size
  };
}

void Font::loadAllGlyphs() {
  FT_UInt i;

  glyphs.resize(face->num_glyphs);

  for(FT_ULong c = FT_Get_First_Char(face, &i); i != 0; c = FT_Get_Next_Char(face, c, &i)) {
    glyphs.at(i) = loadGlyph(i);
  }
}

Font::Glyph const& Font::getChar(FT_ULong charcode) const {
  FT_UInt i = FT_Get_Char_Index(face, charcode);
  return glyphs.at(i);
}

void Font::drawGlyph(GLuint tex, vec3 pos, vec2 size) const {
  // TODO: use fixed quad vertices and compute model matrix instead.
  GLfloat quadPos[6][3] = {
      { pos.x+size.x, pos.y+size.y, pos.z },
      { pos.x, pos.y+size.y, pos.z },
      { pos.x, pos.y, pos.z },

      { pos.x+size.x, pos.y+size.y, pos.z },
      { pos.x, pos.y, pos.z },
      { pos.x+size.x, pos.y, pos.z },
  };

  static const GLfloat quadTex[6][2] = {
      { 1.0f, 0.0f },
      { 0.0f, 0.0f },
      { 0.0f, 1.0f },

      { 1.0f, 0.0f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
  };

  // update vbo and render
  glBindVertexArray(vao);

  GLint texSampler = Shader::getActive()->getUniform("text");
  glUniform1i(texSampler, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadPos), quadPos);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(quadPos), sizeof(quadTex), quadTex);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Font::draw(std::string text, vec3 pos, float scale, vec4 color) const {
  if(text.size() == 0) return;
  Shader* shader = Shader::getActive();
  glUniform4fv(shader->getUniform("color"), 1, glm::value_ptr(color));

  for(char32_t c : UTF8StringAdaptator(text)) {
    Glyph const& g = getChar(c);

    float xpos = pos.x + g.bearing.x * scale;
    float ypos = pos.y - (g.size.y - g.bearing.y) * scale;

    auto gpos = vec3(xpos, ypos, pos.z);
    auto gsize = vec2(g.size) * scale;

    drawGlyph(g.tex, gpos, gsize);

    pos.x += g.advance * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
