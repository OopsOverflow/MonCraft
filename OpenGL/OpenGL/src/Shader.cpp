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

GLuint Shader::getLocation(ShaderLocation loc) const { return locations[loc]; }

GLint Shader::getUniformLocation(const std::string &location) {
  return glGetUniformLocation(program, location.c_str());
}

// --------------- private ---------------

void Shader::initLocations() {
  glUseProgram(program);

#define ATTRLOC(str) glGetAttribLocation(program, str)
#define UNILOC(str) glGetUniformLocation(program, str)
  locations[VERTEX_POSITION] = ATTRLOC("v_position");
  locations[VERTEX_COLOR] = ATTRLOC("v_color");
  locations[VERTEX_NORMAL] = ATTRLOC("v_normal");
  locations[VERTEX_TEXTURE] = UNILOC("v_texture");
  locations[MATRIX_MODEL_VIEW] = UNILOC("m_modelView");
  locations[MATRIX_PROJECTION] = UNILOC("m_projection");
  locations[MATRIX_NORMAL] = UNILOC("m_normal");
  locations[CAMERA_CENTER] = UNILOC("c_center");
#undef ATTRLOC
#undef UNILOC

  std::cout << "---- SHADER ----" << std::endl
            << "v_position: " << locations[VERTEX_POSITION] << std::endl
            << "v_color: " << locations[VERTEX_COLOR] << std::endl
            << "v_normal: " << locations[VERTEX_NORMAL] << std::endl
            << "v_texture: " << locations[VERTEX_TEXTURE] << std::endl
            << "m_modelView: " << locations[MATRIX_MODEL_VIEW] << std::endl
            << "m_projection: " << locations[MATRIX_PROJECTION] << std::endl
            << "m_normal: " << locations[MATRIX_NORMAL] << std::endl
            << "c_center: " << locations[CAMERA_CENTER] << std::endl;
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
