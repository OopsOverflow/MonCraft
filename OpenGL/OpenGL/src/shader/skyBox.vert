#version 330 core
layout (location = 0) in vec3 v_position;

out vec3 txrCoords;

uniform mat4 m_projection;
uniform mat4 m_modelView;
uniform mat4 c_center;

void main()
{
    txrCoords = vec3(m_modelView * vec4(v_position, 1.0));
    vec4 pos = m_projection * m_modelView * vec4(v_position, 1.0);
    gl_Position = pos.xyww;
}
