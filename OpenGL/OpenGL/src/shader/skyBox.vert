#version 300 es
in vec3 v_position;

out vec3 txrCoords;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_projection;

void main()
{
    txrCoords =  normalize(v_position.xyz);
    vec4 pos =  m_projection * m_view * vec4(v_position, 1.0) ;
    gl_Position = pos.xyww;
}
