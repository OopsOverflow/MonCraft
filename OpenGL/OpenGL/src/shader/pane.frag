#version 300 es

precision mediump float;

out vec4 res;

uniform vec4 color;

void main()
{
    res = color;
}
