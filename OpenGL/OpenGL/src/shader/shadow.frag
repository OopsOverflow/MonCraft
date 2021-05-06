#version 330 core


void main()
{
    gl_FragDepth = gl_FragCoord.z;
    gl_FragDepth = .5;
}
