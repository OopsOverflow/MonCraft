#version 430 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture;
layout (location = 3) in float v_occlusion;

layout (location = 0) uniform mat4 m_model;
layout (location = 1) uniform mat4 m_view;
layout (location = 2) uniform mat4 m_projection;
layout (location = 3) uniform mat4 m_normal;
layout (location = 4) uniform mat4 m_shadows[3];

smooth out vec3 vertexPosition;
smooth out vec4 vertexColor;
smooth out vec3 vertexNormal;
smooth out vec2 txrCoords;
smooth out vec3 shadowCoords[3];
smooth out float vertexOcclusion;
smooth out float clipEndZ;

vec3 nsigmoid2(vec3 x, float k) {
  vec3 val = (-pow(x, vec3(3)) + x * 3.0) / 2.0;
  return clamp(val, -1, 1);
}

vec3 nsigmoid1(vec3 x, float k) {
  return 1 / (1 + exp(-x * k)) * 2 - 1;
}

void main() {
  // Pos
  vec4 vertPos4 = m_view * m_model * vec4(v_position, 1.0);
  gl_Position = m_projection * vertPos4;

  // ----
  // gl_Position = vec4(nsigmoid1(gl_Position.xyz, 0.1), gl_Position.w);
  // ----

  vertexPosition = vec3(vertPos4) / vertPos4.w;
  // Normals
  vertexNormal = vec3(m_normal * vec4(v_normal, 0.0));
  // Textures
  txrCoords = v_texture;
  // Occlusion
  vertexOcclusion = v_occlusion;

  // shadows
  clipEndZ = gl_Position.z;
  vec4 shadowCoords4;
  shadowCoords4 = m_shadows[0] * m_model * vec4(v_position, 1.0);
  shadowCoords[0] = vec3(shadowCoords4) / shadowCoords4.w;
  shadowCoords4 = m_shadows[1] * m_model * vec4(v_position, 1.0);
  shadowCoords[1] = vec3(shadowCoords4) / shadowCoords4.w;
  shadowCoords4 = m_shadows[2] * m_model * vec4(v_position, 1.0);
  shadowCoords[2] = vec3(shadowCoords4) / shadowCoords4.w;
  // shadowCoords = nsigmoid1(shadowCoords, 5); // lod for shadows
}
