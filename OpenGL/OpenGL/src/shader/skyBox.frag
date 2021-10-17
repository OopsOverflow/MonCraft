#version 300 es

precision mediump float;

#define UNDERWATER_FLAG 1

out vec4 outputColor;

in vec3 txrCoords;

uniform samplerCube skyboxD;
uniform samplerCube skyboxN;
uniform float blendFactor;
uniform int flags;

void main()
{
    vec4 textureD = texture(skyboxD, txrCoords);
    vec4 textureN = texture(skyboxN, txrCoords);

    outputColor = mix(textureD, textureN, blendFactor);

  if((flags & UNDERWATER_FLAG) == 1) {
    outputColor.rgb *= vec3(127.0 / 255.0, 148.0 / 255.0, 1.0);
  }
}
