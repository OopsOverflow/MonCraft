#version 430 core

precision mediump float;

smooth in vec3 vertexPosition;
smooth in vec3 vertexNormal;
smooth in float vertexOcclusion;
smooth in vec2 txrCoords;
smooth in vec3 shadowCoords;

// COMBAK: for some reason on windows these require an explicit location once at least 1 uniform in the program is explicit
layout(location = 10) uniform vec3 lightDirection;
layout(location = 11) uniform float lightIntensity;

uniform sampler2D textureSampler;
uniform sampler2D shadowSampler;
out vec4 outputColor;

float computeShadow() {
  // if(shadowCoords.z > 1.0)
  //   return 0.0;

  // float bias = 0.0002;
  // float bias = 0.001;
  float bias = 1.0 / textureSize(shadowSampler, 0).x;
  float currentDepth = shadowCoords.z * 0.5 + 0.5;
  vec2 texelSize = 1.0 / textureSize(shadowSampler, 0) / 2;

  // no pcf
  // float pcfDepth = texture(shadowSampler, shadowCoords.xy * 0.5 + 0.5).r;
  // return currentDepth - bias > pcfDepth ? 1.0 : 0.0;

  // pcf
  float shadow = 0.0;
  for(float x = -1.5; x <= 1.5; ++x) {
      for(float y = -1.5; y <= 1.5; ++y) {
          float pcfDepth = texture(shadowSampler, shadowCoords.xy * 0.5 + 0.5 + vec2(x, y) * texelSize).r;
          shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
      }
  }
  shadow /= 16.0;
  return shadow;
}

void main() {
  vec3 normal = normalize(-vertexNormal);
  float lambertian = max(dot(normalize(lightDirection), normal), 0.0);

  // Textures
  outputColor = texture(textureSampler, txrCoords);
  float shadow = 1 - computeShadow();
  outputColor = outputColor * .5 + outputColor * lambertian * lightIntensity * shadow * .5;

  float occl = .7;
  outputColor *= 1.0 - (vertexOcclusion * vertexOcclusion / 9.0) * occl;
}
