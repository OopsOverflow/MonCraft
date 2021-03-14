#version 330 core

precision mediump float;

smooth in vec3 vertexPosition;
smooth in vec4 vertexColor;
smooth in vec3 vertexNormal;

out vec4 outputColor;

void main() {

  vec3 colorized = vertexColor.xyz;
  outputColor = vec4(colorized, vertexColor.a);

  outputColor = vec4(1.0);
}
