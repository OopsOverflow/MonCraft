
#version 430 core

in vec3 v_position;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_projection;

out vec4 vertexColor;

void main() {
  // Pos
  vec4 vertPos4 = m_view * m_model * vec4(v_position, 1.0);
  gl_Position = m_projection * vertPos4;
  
  const float maxDist = 64;
  float alpha = 1 - length(vertPos4.xyz) / maxDist;
  vertexColor = vec4(1.0, 1.0, 0.0, max(alpha, 0));
}

