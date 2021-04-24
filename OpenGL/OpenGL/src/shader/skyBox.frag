#version 430 core
out vec4 outputColor;

in vec3 txrCoords;
//in float v_fog_depth;

uniform samplerCube skybox;

//const vec4 fog_color = vec4(0.5f, 0.8f, 0.9f, 1.0f);
//const float fog_near = 100.0f;
//const float fog_far = 128.0f;

void main()
{    
    outputColor = texture(skybox, txrCoords);

    //float fog = smoothstep(fog_near, fog_far, v_fog_depth);
    //outputColor = mix(outputColor, fog_color, fog);
}