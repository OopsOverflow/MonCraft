#pragma once

#include "bitmap.hpp"
#include "../gl/Shader.hpp"

/**
 * Draws a bitmap on screen. Mostly for debugging purposes.
 */

class BitmapTex
{
public:
  BitmapTex();
  virtual ~BitmapTex();

  /**
   * Generates the internal texture from a bitmap.
   */
  void generate(Bitmap const& map);

  /**
  * Draws the texture, filling the gl viewport.
  */
  void draw();

private:
  GLuint vao;
  GLuint vbo;
  GLuint tex;
  static Shader *shader;
};
