#version 430 core
layout (location = 0) in vec3 v_position;
layout (location = 2) in vec2 v_texture;

layout (location = 0) uniform mat4 m_model;
layout (location = 1) uniform mat4 m_view;
layout (location = 2) uniform mat4 m_projection;

out vec2 texCoords;

void main() {
  gl_Position = m_projection * m_view * m_model * vec4(v_position, 1.0);
  texCoords = v_texture;
}
