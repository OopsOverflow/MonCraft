#version 430 core
in vec2 texCoords;
out vec4 res;

uniform sampler2D text;
uniform vec4 color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
    res = color * sampled;
}
