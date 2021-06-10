#version 430 core
layout (location = 0) in vec2 v_position;

layout (location = 0) uniform mat4 m_model;
layout (location = 1) uniform mat4 m_view;
layout (location = 2) uniform mat4 m_projection;

out vec2 texCoords;

void main() {
  gl_Position = m_projection * m_view * m_model * vec4(v_position, 0.0, 1.0);
}
