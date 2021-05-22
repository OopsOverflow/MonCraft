#include "Font.hpp"
#include "debug/Debug.hpp"
#include <stdexcept>
#include <array>

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

void Font::loadAllGlyphs() {
  for (unsigned char c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))   {
      std::cout << "[Warning] Failed to load Glyph " << c << std::endl;
      continue;
    }

    // generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RED,
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      0, GL_RED, GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer
    );

    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // now store character for later use
    Character character = {
      texture,
      ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      face->glyph->advance.x
    };
    characters.emplace(c, character);
  }
}

void Font::draw(std::string text, vec3 pos, float scale, vec3 color) const {
  Shader* shader = Shader::getActive();
  glUniform3f(shader->getUniformLocation("textColor"), color.x, color.y, color.z);

  for(auto c : text) {
    Character const& ch = characters.at(c);

    float xpos = pos.x + ch.bearing.x * scale;
    float ypos = pos.y - (ch.size.y - ch.bearing.y) * scale;

    // if(anchorX == TextAnchor::END) xpos +=

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    GLfloat quadPos[6][3] = {
        { xpos+w, ypos+h, pos.z },
        { xpos, ypos+h, pos.z },
        { xpos, ypos, pos.z },

        { xpos+w, ypos+h, pos.z },
        { xpos, ypos, pos.z },
        { xpos+w, ypos, pos.z },
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

    GLint texSampler = Shader::getActive()->getUniformLocation("text");
    glUniform1i(texSampler, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ch.tex);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadPos), quadPos);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(quadPos), sizeof(quadTex), quadTex);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    pos.x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
