#version 300 es

in vec2 vPosition;
uniform mat2 uMvp;
smooth out vec2 vertexPosition;

void main() {
  vertexPosition = (vPosition + vec2(1.0)) / 2.0;
  gl_Position = vec4(uMvp * vPosition, -1.0, 1.0);
}
