#version 300 es

precision mediump float;

out vec4 outputColor;
uniform sampler2D uTexture;

in vec2 vertexPosition;

void main() {
  outputColor = texture(uTexture, vertexPosition);
}
