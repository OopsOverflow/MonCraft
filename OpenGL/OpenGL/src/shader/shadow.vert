#version 430 core
layout (location = 0) in vec3 v_position;
layout (location = 0) uniform mat4 m_model;
layout (location = 4) uniform mat4 m_shadows;

void main()
{
  gl_Position = m_shadows * m_model * vec4(v_position, 1.0);
}
