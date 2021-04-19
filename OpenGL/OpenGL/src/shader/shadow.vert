#version 430 core
layout (location = 0) in vec3 v_position;
layout (location = 0) uniform mat4 m_model;
layout (location = 4) uniform mat4 m_shadows;

vec3 nsigmoid1(vec3 x, float k) {
  return 1 / (1 + exp(-x * k)) * 2 - 1;
}

vec3 nsigmoid2(vec3 x, float k) {
  vec3 val = (-pow(x, vec3(3)) + 3 * x) / 2;
  return clamp(val, -1, 1);
}

void main()
{
  gl_Position = m_shadows * m_model * vec4(v_position, 1.0);
  // gl_Position = vec4(nsigmoid1(gl_Position.xyz, 5), gl_Position.w); // lod for shadows
}
