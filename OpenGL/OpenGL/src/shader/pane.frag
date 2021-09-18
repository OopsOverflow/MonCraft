#version 430 core

precision mediump float;

out vec4 res;

uniform vec4 color;

void main()
{
    res = color;
}
