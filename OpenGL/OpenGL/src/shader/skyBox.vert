#version 430 core
layout (location = 0) in vec3 v_position;

out vec3 txrCoords;

layout (location = 0) uniform mat4 m_model;
layout (location = 1) uniform mat4 m_view;
layout (location = 2) uniform mat4 m_projection;

void main()
{
    txrCoords =  normalize(v_position.xyz);
    vec4 pos =  m_projection * m_view * vec4(v_position, 1.0) ;
    gl_Position = pos.xyww;
}
