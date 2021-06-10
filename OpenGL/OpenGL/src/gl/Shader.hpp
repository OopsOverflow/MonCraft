#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>

enum ShaderLocation {
	// vertex attributes
	VERTEX_POSITION = 0,
	VERTEX_NORMAL = 1,
	VERTEX_TEXTURE = 2,
	VERTEX_OCCLUSION = 3,
	VERTEX_NORMALTEXTURE = 4,

	// uniform matrices
	MATRIX_MODEL = 0,
	MATRIX_VIEW = 1,
	MATRIX_PROJECTION = 2,
	MATRIX_NORMAL = 3,
	MATRIX_SHADOWS = 4,

	// texture sampler uniforms
	TEXTURE_COLOR = 0,
	TEXTURE_NORMAL = 1,
	TEXTURE_SHADOW0 = 2,
	TEXTURE_SHADOW1 = 3,
	TEXTURE_SHADOW2 = 4,

	// other uniforms
	CAMERA_CENTER = -1,
	SUN_TIME = -1

};

/**
 * A Shader creates a gl program and manages its lifetime.
 */

class Shader {
public:

	/**
	 * Creates a Shader from the vertex and fragment filepath given.
	 * The Construction will not fail if the compilation fails, but will print
	 * the error to stdout.
	 */
  Shader(const char *vertexPath, const char *fragmentPath);

	/**
	 * Activates the Shader.
	 * basically calls glUseProgram()
	 */
  void activate();

	/**
	 * Gets a uniform location corresponding to the name provided.
	 * -1 means that the location was not found / was optimized away by the
	 * shader compiler.
	 */
  GLint getUniformLocation(const std::string &location) const;

	/**
	 * TODO doc
	 */
  void bindTexture(ShaderLocation textureLocation, GLuint texture) const;

	/**
	 * Gets the shader currently active.
	 */
  static Shader *getActive();

	/**
	 * the shader program. <!\ This is readonly.
	 */
  GLuint program;

private:
  static Shader *activeShader;


  void initLocations();

  std::string readFile(const std::string &path);
  GLuint compileShader(int type, const std::string &shader);
  GLuint createShader(int type, const char *path);
  GLuint createProgram(GLuint vertex, GLuint fragment);
};
