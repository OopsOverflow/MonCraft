#version 300 es

precision mediump float;

out vec4 outputColor;

in vec3 txrCoords;

uniform samplerCube skyboxD;
uniform samplerCube skyboxN;
uniform float blendFactor;
uniform int underWater; //glUniform1b :(

void main()
{
    vec4 textureD = texture(skyboxD, txrCoords);
    vec4 textureN = texture(skyboxN, txrCoords);

    outputColor = mix(textureD, textureN, blendFactor);

  if(underWater == 1) {
    outputColor.rgb *=vec3(127.0 / 255.0, 148.0 / 255.0, 1.0);
  }
}
