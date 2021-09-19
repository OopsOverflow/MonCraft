#version 300 es

precision mediump float;

smooth in vec3 vertexPosition;
smooth in vec3 fogColor;

out vec4 outputColor;

uniform vec3 lightDirection;
uniform float lightIntensity;
uniform float sunTime;

float random (in vec2 _st) {
    return fract(sin(dot(_st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 _st) {
    vec2 i = floor(_st);
    vec2 f = fract(_st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

#define NUM_OCTAVES 5

float fbm ( in vec2 _st) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100.0);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5),
                    -sin(0.5), cos(0.50));
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise(_st);
        _st = rot * _st * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}

void main()
{
        // Noise Sampling Bovine Excrement
    vec2 st = gl_FragCoord.xy/100.840f;
    // st += st * abs(sin(sunTime*0.1)*3.0);
    vec3 fcolor = vec3(0.0);

    vec2 q = vec2(0.);
    q.x = fbm( st + 0.00*sunTime);
    q.y = fbm( st + vec2(1.0));

    vec2 r = vec2(0.);
    r.x = fbm( st + 1.0*q + vec2(1.7,9.2)+ 0.15*sunTime );
    r.y = fbm( st + 1.0*q + vec2(8.3,2.8)+ 0.126*sunTime);

    float f = fbm(st+r);

    fcolor = mix(fogColor,
                vec3(0.711,0.707,0.730),
                clamp((f*f)*4.408,0.0,1.0));

    fcolor = mix(fcolor,
                vec3(0,0,0.164706),
                clamp(length(q),0.0,-0.984));

    fcolor = mix(fcolor,
                vec3(0.799,1.000,0.981),
                clamp(length(r.x),0.0,1.0));

    vec4 fbmColor = vec4((f*f*f+.6*f*f+.5*f)*fcolor,0.);

  // Fog Calc
    float b = 0.0004f;  // fallof of the fog density
    float c = 0.5f;     // integration constant, varies distance of fallof
    float distance = (vertexPosition.y);
    float fogAmounty = c * exp(vertexPosition.z*b) * (1.0f-exp( vertexPosition.z*lightDirection.y*b ))/lightDirection.y;
    c =1.0f;
    b = 0.0009f;
    float fogAmountz = c * exp(-vertexPosition.z*b) * (1.0-exp( -vertexPosition.z*lightDirection.z*b ))/lightDirection.y;

    outputColor = mix( outputColor, fbmColor, fogAmountz + fogAmounty);
    outputColor = vec4(1.0,0.0,0.0,0.5);
}
