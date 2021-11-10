#version 300 es

in vec2 v_position;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_projection;

out vec2 texCoords;

void main() {
  gl_Position = m_projection * m_view * m_model * vec4(v_position, 0.0, 1.0);
  texCoords = v_position;
  texCoords.y = 1.0f - texCoords.y;
}
