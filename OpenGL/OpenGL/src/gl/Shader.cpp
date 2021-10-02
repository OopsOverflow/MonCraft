#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#define SHADER_ENABLE_DEBUG 0

Shader *Shader::activeShader = nullptr;

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  GLuint vertex, fragment;
  vertex = createShader(GL_VERTEX_SHADER, vertexPath);
  fragment = createShader(GL_FRAGMENT_SHADER, fragmentPath);
  program = createProgram(vertex, fragment);
  initLocations();
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::activate() {
  glUseProgram(program);
  activeShader = this;
}

Shader *Shader::getActive() { return activeShader; }

GLint Shader::getUniform(const std::string &location) const {
  return glGetUniformLocation(program, location.c_str());
}

GLint Shader::getUniform(ShaderLocation location) const {
  return uniformLookup[location];
}

void Shader::bindTexture(ShaderLocation textureLocation, GLuint texture) const {
  glActiveTexture(GL_TEXTURE0 + textureLocation - TEXTURE_COLOR);
  glBindTexture(GL_TEXTURE_2D, texture);
}

// --------------- private ---------------

void Shader::initLocations() {
  glUseProgram(program);

  for(size_t i = 0; i < uniformLookup.size(); i++)
    uniformLookup[i] = -1;

  uniformLookup[MATRIX_MODEL] = getUniform("m_model");
  uniformLookup[MATRIX_VIEW] = getUniform("m_view");
  uniformLookup[MATRIX_PROJECTION] = getUniform("m_projection");
  uniformLookup[MATRIX_NORMAL] = getUniform("m_normal");
  uniformLookup[MATRIX_SHADOWS] = getUniform("m_shadows");

  uniformLookup[TEXTURE_COLOR] = getUniform("t_color");
  uniformLookup[TEXTURE_NORMAL] = getUniform("t_normal");
  uniformLookup[TEXTURE_SHADOW0] = getUniform("t_shadow[0]");
  uniformLookup[TEXTURE_SHADOW1] = getUniform("t_shadow[1]");
  uniformLookup[TEXTURE_SHADOW2] = getUniform("t_shadow[2]");

  glUniform1i(uniformLookup[TEXTURE_COLOR], 0);
  glUniform1i(uniformLookup[TEXTURE_NORMAL], 1);
  glUniform1i(uniformLookup[TEXTURE_SHADOW0], 2);
  glUniform1i(uniformLookup[TEXTURE_SHADOW1], 3);
  glUniform1i(uniformLookup[TEXTURE_SHADOW2], 4);

  #if defined(DEBUG) && SHADER_ENABLE_DEBUG == 1
    #define ATTRLOC(str) glGetAttribLocation(program, str)
      std::cout << "---- SHADER ----" << std::endl
                << "v_position: " << ATTRLOC("v_position") << std::endl
                << "v_color: " << ATTRLOC("v_color") << std::endl
                << "v_normal: " << ATTRLOC("v_normal") << std::endl
                << "v_texture: " << ATTRLOC("v_texture") << std::endl
                << "v_occlusion: " << ATTRLOC("v_occlusion") << std::endl
                << "v_normalMap: " << ATTRLOC("v_normalMap") << std::endl
                << "m_model: " << uniformLookup[MATRIX_MODEL] << std::endl
                << "m_view: " << uniformLookup[MATRIX_VIEW] << std::endl
                << "m_projection: " << uniformLookup[MATRIX_PROJECTION] << std::endl
                << "m_normal: " << uniformLookup[MATRIX_NORMAL] << std::endl
                << "m_shadows: " << uniformLookup[MATRIX_SHADOWS] << std::endl
                << "t_color: " << uniformLookup[TEXTURE_COLOR] << std::endl
                << "t_normal: " << uniformLookup[TEXTURE_NORMAL] << std::endl
                << "t_shadow0: " << uniformLookup[TEXTURE_SHADOW0] << std::endl
                << "t_shadow1: " << uniformLookup[TEXTURE_SHADOW1] << std::endl
                << "t_shadow2: " << uniformLookup[TEXTURE_SHADOW2] << std::endl;
    #undef ATTRLOC
  #endif
}

std::string Shader::readFile(const std::string &path) {
  std::ifstream file;
  std::string contents;

  // ensure file can throw
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    file.open(path);
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    contents = stream.str();
  } catch (std::ifstream::failure & /* e */) {
    std::cout << "Failed to read file: " << path << std::endl;
  }

  return contents;
}

GLuint Shader::compileShader(int type, const std::string &contents) {
  GLuint shader;
  int success;

  shader = glCreateShader(type);
  const char *c_contents = contents.c_str();
  glShaderSource(shader, 1, &c_contents, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "Compilation failure in "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << "shader: " << infoLog << std::endl;
  };

  return shader;
}

GLuint Shader::createShader(int type, const char *path) {
  std::string contents;
  GLuint shader;

  contents = readFile(path);
  shader = compileShader(type, contents);

  return shader;
}

GLuint Shader::createProgram(GLuint vertex, GLuint fragment) {
  GLuint program;
  int success;

  program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glBindAttribLocation(program, 0, "v_position");
  glBindAttribLocation(program, 1, "v_normal");
  glBindAttribLocation(program, 2, "v_texture");
  glBindAttribLocation(program, 3, "v_occlusion");
  glBindAttribLocation(program, 4, "v_normalMap");
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "Linking failure in program: " << infoLog << std::endl;
  }

  return program;
}
