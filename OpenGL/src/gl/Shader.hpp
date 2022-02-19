#pragma once

#include <GL/glew.h>
#include <string>
#include <array>

enum ShaderLocation {
	// other uniforms
	CAMERA_CENTER = -1,
	SUN_TIME = -1,

	// vertex attributes
	VERTEX_POSITION,
	VERTEX_NORMAL,
	VERTEX_TEXTURE,
	VERTEX_OCCLUSION,
	VERTEX_NORMALTEXTURE,

	// uniform matrices
	MATRIX_MODEL,
	MATRIX_VIEW,
	MATRIX_PROJECTION,
	MATRIX_NORMAL,
	MATRIX_SHADOWS,

	// texture sampler uniforms
	TEXTURE_COLOR,
	TEXTURE_NORMAL,
	TEXTURE_SHADOW0,
	TEXTURE_SHADOW1,
	TEXTURE_SHADOW2,

	// count (must be last)
	LOCATION_COUNT
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
  GLint getUniform(const std::string &location) const;

  GLint getUniform(ShaderLocation location) const;

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
	std::array<GLint, LOCATION_COUNT> uniformLookup;

  void initLocations();

  std::string readFile(const std::string &path);
  GLuint compileShader(int type, const std::string &shader);
  GLuint createShader(int type, const char *path);
  GLuint createProgram(GLuint vertex, GLuint fragment);
};
