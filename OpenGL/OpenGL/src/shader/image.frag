#version 300 es

precision mediump float;

in vec2 texCoords;

uniform sampler2D uTexture;
uniform vec2 scale;
uniform mat4 m_texture;


out vec4 outputColor;

void main() {

  vec2 offset = vec2(m_texture[3][0], m_texture[3][1]);
  vec2 size = vec2(m_texture[0][0], m_texture[1][1]);


  outputColor = texture(uTexture, vec2(offset.x, 1.0f-(offset.y+size.y)) + size * fract(texCoords * scale ) );
}
