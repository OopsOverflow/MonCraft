#version 330 core

in vec3 v_position;
in vec4 v_color;
in vec3 v_normal;

uniform mat4 m_modelView;
uniform mat4 m_projection;
uniform mat4 m_normal;

smooth out vec3 vertexPosition;
smooth out vec4 vertexColor;
smooth out vec3 vertexNormal;

void main() {
  vec4 vertPos4 = m_modelView * vec4(v_position, 1.0);

  gl_Position = m_projection * vertPos4;

  vertexPosition = vec3(vertPos4) / vertPos4.w;
  vertexNormal = vec3(m_normal * vec4(v_normal, 0.0));
  vertexColor = v_color;
}
