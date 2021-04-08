#version 330 core

layout (location = 0) in vec3 v_position; // Explicit 
layout (location = 1) in vec4 v_color;	 // to be changed later
layout (location = 2) in vec2 v_texture;
in vec3 v_normal;

uniform mat4 m_modelView;
uniform mat4 m_projection;
uniform mat4 m_normal;

smooth out vec3 vertexPosition;
smooth out vec4 vertexColor;
smooth out vec3 vertexNormal;
smooth out vec2 txrCoords;

void main() {
  // Pos
  vec4 vertPos4 = m_modelView * vec4(v_position, 1.0);
  gl_Position = m_projection * vertPos4;
  vertexPosition = vec3(vertPos4) / vertPos4.w;
  // Normals
  vertexNormal = vec3(m_normal * vec4(v_normal, 0.0));
  // Color
  vertexColor = v_color;
  // Textures
  txrCoords = v_texture;
}
