#version 430 core
in vec3 v_position;
uniform mat4 m_model;
uniform mat4 m_shadows;

vec3 nsigmoid1(vec3 x, float k) {
  return vec3(1.0) / (vec3(1.0) + exp(-x * k)) * 2.0 - 1.0;
}

vec3 nsigmoid2(vec3 x, float k) {
  vec3 val = (-pow(x, vec3(3.0)) + 3.0 * x) / 2.0;
  return clamp(val, -1.0, 1.0);
}

void main()
{
  gl_Position = m_shadows * m_model * vec4(v_position, 1.0);
  // gl_Position = vec4(nsigmoid1(gl_Position.xyz, 5), gl_Position.w); // lod for shadows
}
