#version 430 core

precision mediump float;

in vec4 vertexColor;

out vec4 outputColor;

void main() {
    outputColor = vertexColor;
}
