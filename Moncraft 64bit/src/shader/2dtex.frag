#version 330 core

precision mediump float;

out vec4 outputColor;
uniform sampler2D uTexture;

smooth in vec2 vertexPosition;

void main() {
  outputColor = texture(uTexture, vertexPosition);
}
