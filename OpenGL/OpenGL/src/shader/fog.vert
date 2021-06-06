#version 430 core
layout (location = 0) in vec3 v_position;

layout (location = 0) uniform mat4 m_model;
layout (location = 1) uniform mat4 m_view;
layout (location = 2) uniform mat4 m_projection;

layout(location = 10) uniform vec3 lightDirection;

smooth out vec3 vertexPosition;
smooth out vec3 fogColor;


out vec2 texCoords;

void main() {
  vec4 vertPos4 = m_view * m_model * vec4(v_position, 1.0);

    // fog
   float sunAmount = max( dot(vec3(m_view), lightDirection), 0.0 );
   vec3  fogColor  = mix( vec3(0.5,0.6,0.7), // bluish
                           vec3(1.0,0.9,0.7), // yellowish
                           pow(sunAmount,8.0) );
}

// Move calc to cpp program
// send calc color to vertex shader
// mix fog color, block color and skybox color

// Idea 1 : sep
// Hours wasted = 1

// Idea 2 : frame buffer

