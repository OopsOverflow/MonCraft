#include "bitmapTex.hpp"
#include <glm/gtc/type_ptr.hpp>

Shader *BitmapTex::shader = nullptr;

BitmapTex::BitmapTex() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenTextures(1, &tex);

  if (!shader) {
    shader = new Shader("src/shader/2dtex.vert", "src/shader/2dtex.frag");
  }
  
  // GLint u_mvp = glGetUniformLocation(shader->program, "uMvp");
  // GLint v_position = glGetAttribLocation(shader->program, "vPosition");
  // std::cout << "---- SHADER ---- " << std::endl;
  // std::cout << "u_mvp " << u_mvp << std::endl;
  // std::cout << "v_position " << v_position << std::endl;
}

void BitmapTex::generate(Bitmap const& map) {
  glm::vec2 verts[] = {
      glm::vec2{-1, -1},
      glm::vec2{1, -1},
      glm::vec2{1, 1},

      glm::vec2{-1, -1},
      glm::vec2{1, 1},
      glm::vec2{-1, 1},
  };

  glm::mat2 mvp(1.f);

  shader->activate();
  GLint u_mvp = glGetUniformLocation(shader->program, "uMvp");
  GLint v_position = glGetAttribLocation(shader->program, "vPosition");

  glBindVertexArray(vao);
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(verts), verts, GL_STATIC_DRAW);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, map.width, map.height, 0, GL_RGB, GL_UNSIGNED_BYTE, map.c_ptr());
    glActiveTexture(GL_TEXTURE0);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glVertexAttribPointer(v_position, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(v_position);
    glUniformMatrix2fv(u_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
  }
  glBindVertexArray(0);
}

void BitmapTex::draw() {
  shader->activate();
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

BitmapTex::~BitmapTex() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteTextures(1, &tex);
}
