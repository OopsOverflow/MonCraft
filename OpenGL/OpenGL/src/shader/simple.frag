#version 430 core

precision mediump float;

smooth in vec3 vertexPosition;
smooth in vec3 vertexNormal;
smooth in float vertexOcclusion;
smooth in vec2 txrCoords;
smooth in vec3 shadowCoords[3];

// COMBAK: for some reason on windows these require an explicit location once at least 1 uniform in the program is explicit
layout(location = 10) uniform vec3 lightDirection;
layout(location = 11) uniform float lightIntensity;

uniform sampler2D textureSampler;
uniform sampler2D shadowSampler[3];
uniform float clipCascadeEndZ[3];
out vec4 outputColor;

float computeShadow(int i) {
  vec3 normal = normalize(vertexNormal);
  float dotNormal = dot(normalize(lightDirection), normal);

  // float bias = 0.0;
  float bmin = 0.0001;
  float bmax = 0.0005;
  float bias = max(bmax * (1.0 - dotNormal), bmin);
  float currentDepth = shadowCoords[i].z * 0.5 + 0.5;
  vec2 texelSize = 1.0 / textureSize(shadowSampler[i], 0) / 2;

  // no pcf
  // float pcfDepth = texture(shadowSampler[i], shadowCoords[i].xy * 0.5 + 0.5).r;
  // return currentDepth - bias > pcfDepth ? 1.0 : 0.0;

  // pcf
  float shadow = 0.0;
  for(float x = -1.5; x <= 1.5; ++x) {
      for(float y = -1.5; y <= 1.5; ++y) {
          float pcfDepth = texture(shadowSampler[i], shadowCoords[i].xy * 0.5 + 0.5 + vec2(x, y) * texelSize).r;
          shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
      }
  }
  shadow /= 16.0;
  return shadow;
}

float linearizeDepth(float depth) { // https://learnopengl.com/Advanced-OpenGL/Depth-testing
    float near = 0.1;
    float far = 200.0;
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near)) / far;
}

void main() {
  vec3 normal = normalize(vertexNormal);
  float dotNormal = dot(normalize(lightDirection), normal);
  float lambertian = max(-dotNormal, 0.0);

  // Textures
  outputColor = texture(textureSampler, txrCoords);

  // shadow

  float shadow = 0.0;

  for (int i = 0 ; i < 3; i++) {
    if (texture(shadowSampler[i], shadowCoords[i].xy * 0.5 + 0.5).r != 1.0) {
      shadow = 1 - computeShadow(i);
      break;
    }
  }

  outputColor.xyz = outputColor.xyz * .6 + outputColor.xyz * lightIntensity * lambertian * shadow * .4;

  float occl = .7;
  outputColor.xyz *= 1.0 - (vertexOcclusion * vertexOcclusion / 9.0) * occl;

  // show in which shadow cascade we are
  // for (int i = 0 ; i < 3 ; i++) {
  //   if (gl_FragCoord.z <= clipCascadeEndZ[i]) {
  //     outputColor[i] += 0.2;
  //   }
  // }

  if(outputColor.a < 0.1) {
    discard;
  }
}
