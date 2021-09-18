#version 430 core
in vec3 v_position;
in vec2 v_texture;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_projection;

out vec2 texCoords;

void main() {
  gl_Position = m_projection * m_view * m_model * vec4(v_position, 1.0);
  texCoords = v_texture;
}
