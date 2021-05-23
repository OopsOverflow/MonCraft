#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>

Shader *Shader::activeShader = nullptr;

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  GLuint vertex, fragment;
  vertex = createShader(GL_VERTEX_SHADER, vertexPath);
  fragment = createShader(GL_FRAGMENT_SHADER, fragmentPath);
  program = createProgram(vertex, fragment);
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  initLocations();
}

void Shader::activate() {
  glUseProgram(program);
  activeShader = this;
}

Shader *Shader::getActive() { return activeShader; }

GLint Shader::getUniformLocation(const std::string &location) const {
  return glGetUniformLocation(program, location.c_str());
}

void Shader::bindTexture(ShaderLocation textureLocation, GLuint texture) const {
  glActiveTexture(GL_TEXTURE0 + textureLocation);
  glBindTexture(GL_TEXTURE_2D, texture);
}

// --------------- private ---------------

void Shader::initLocations() {
  glUseProgram(program);

  GLint t_color = getUniformLocation("t_color");
  GLint t_normal = getUniformLocation("t_normal");
  GLint t_shadow0 = getUniformLocation("t_shadow[0]");
  GLint t_shadow1 = getUniformLocation("t_shadow[1]");
  GLint t_shadow2 = getUniformLocation("t_shadow[2]");
  glUniform1i(t_color, 0);
  glUniform1i(t_normal, 1);
  glUniform1i(t_shadow0, 2);
  glUniform1i(t_shadow1, 3);
  glUniform1i(t_shadow2, 4);

#define ATTRLOC(str) glGetAttribLocation(program, str)
#define UNILOC(str) glGetUniformLocation(program, str)
  std::cout << "---- SHADER ----" << std::endl
            << "v_position: " << ATTRLOC("v_position") << std::endl
            << "v_color: " << ATTRLOC("v_color") << std::endl
            << "v_normal: " << ATTRLOC("v_normal") << std::endl
            << "v_texture: " << ATTRLOC("v_texture") << std::endl
            << "v_occlusion: " << ATTRLOC("v_occlusion") << std::endl
            << "v_normalMap: " << ATTRLOC("v_normalMap") << std::endl
            << "m_model: " << UNILOC("m_model") << std::endl
            << "m_view: " << UNILOC("m_view") << std::endl
            << "m_projection: " << UNILOC("m_projection") << std::endl
            << "m_normal: " << UNILOC("m_normal") << std::endl
            << "m_shadows: " << UNILOC("m_shadows") << std::endl
            << "t_color: " << t_color << std::endl
            << "t_normal: " << t_normal << std::endl
            << "t_shadow0: " << t_shadow0 << std::endl
            << "t_shadow1: " << t_shadow1 << std::endl
            << "t_shadow2: " << t_shadow2 << std::endl;
  #undef ATTRLOC
  #undef UNILOC
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
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "Linking failure in program: " << infoLog << std::endl;
  }

  return program;
}
