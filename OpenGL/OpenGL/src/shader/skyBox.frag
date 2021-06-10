#version 430 core
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

  if(underWater == 1){
    outputColor.rgb *=vec3(127.0f/255, 148.0f/255, 1.0f) ;
  }
}