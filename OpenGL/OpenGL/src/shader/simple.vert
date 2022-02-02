#version 430 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texture;
in float v_occlusion;
in vec2 v_normalMap;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_projection;
uniform mat4 m_normal;
uniform mat4 m_shadows[3];

out vec3 vertexPosition;
out vec4 vertexColor;
out vec3 vertexNormal;
out vec2 texCoords;
out vec2 normalCoords;
out vec3 shadowCoords[3];
out mat3 TBN;
out float vertexOcclusion;
out float clipEndZ;


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
  texCoords = v_texture;
  // Occlusion
  vertexOcclusion = v_occlusion;
  // NormalCoords
  normalCoords = v_normalMap;

  //normalMatrix
  vec3 T= vec3(m_normal * vec4(v_normal.zxy, 1.0));
  vec3 B= vec3(m_normal * vec4(v_normal.yzx, 1.0));
  vec3 N= vec3(m_normal * vec4(v_normal, 1.0));
  TBN = mat3(T, B, N);

  // shadows
  clipEndZ = gl_Position.z;
  vec4 shadowCoords4;
  shadowCoords4 = m_shadows[0] * m_model * vec4(v_position, 1.0);
  shadowCoords[0] = vec3(shadowCoords4) / shadowCoords4.w;
  shadowCoords4 = m_shadows[1] * m_model * vec4(v_position, 1.0);
  shadowCoords[1] = vec3(shadowCoords4) / shadowCoords4.w;
  shadowCoords4 = m_shadows[2] * m_model * vec4(v_position, 1.0);
  shadowCoords[2] = vec3(shadowCoords4) / shadowCoords4.w;

}
