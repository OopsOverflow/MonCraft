#version 430 core
in vec3 v_position;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_projection;

smooth out vec3 vertexPosition;
smooth out vec3 fogColor;


out vec2 texCoords;

void main() {
}

// Move calc to cpp program
// send calc color to vertex shader
// mix fog color, block color and skybox color

// Idea 1 : sep
// Hours wasted = 1

// Idea 2 : frame buffer
