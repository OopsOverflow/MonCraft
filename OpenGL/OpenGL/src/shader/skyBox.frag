#version 330 core
out vec4 outputColor;

in vec3 txrCoords;

uniform samplerCube skybox;
uniform int underWater; //glUniform1b :(

void main()
{    
    outputColor = texture(skybox, txrCoords);
  if(underWater == 1){
    outputColor.rgb *=vec3(127.0f/255, 148.0f/255, 1.0f) ;
  }
}