#version 300 es

precision mediump float;

smooth in vec2 texCoords;

out vec4 outputColor;
uniform sampler2D uTexture;



void main() {
  outputColor = texture(uTexture, texCoords);
}
