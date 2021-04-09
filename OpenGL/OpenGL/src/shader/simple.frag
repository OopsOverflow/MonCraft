#version 330 core

precision mediump float;

smooth in vec3 vertexPosition;
smooth in vec4 vertexColor;
smooth in vec3 vertexNormal;
smooth in vec2 txrCoords;

uniform sampler2D textureSampler;
out vec4 outputColor;

void main() {

  // Color
  vec3 colorized = vertexColor.xyz;
  outputColor = vec4(colorized, vertexColor.a);
  // Textures
  outputColor = texture ( textureSampler, txrCoords );

}
