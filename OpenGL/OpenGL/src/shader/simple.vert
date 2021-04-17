#version 430 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture;
layout (location = 3) in float v_occlusion;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_projection;
uniform mat4 m_normal;
layout (location = 4) uniform mat4 m_shadows;

smooth out vec3 vertexPosition;
smooth out vec4 vertexColor;
smooth out vec3 vertexNormal;
smooth out vec2 txrCoords;
smooth out vec3 shadowCoords;
smooth out float vertexOcclusion;

void main() {
  // Pos
  vec4 vertPos4 = m_view * m_model * vec4(v_position, 1.0);
  gl_Position = m_projection * vertPos4;
  vertexPosition = vec3(vertPos4) / vertPos4.w;
  // Normals
  vertexNormal = vec3(m_normal * vec4(v_normal, 0.0));
  // Textures
  txrCoords = v_texture;
  // Occlusion
  vertexOcclusion = v_occlusion;

  vec4 shadowCoords4 = m_shadows * m_model * vec4(v_position, 1.0);
  shadowCoords = vec3(shadowCoords4) / shadowCoords4.w;
}
