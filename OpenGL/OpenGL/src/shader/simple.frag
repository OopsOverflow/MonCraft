#version 330 core

precision mediump float;

smooth in vec3 vertexPosition;
smooth in vec4 vertexColor;
smooth in vec3 vertexNormal;
smooth in float vertexOcclusion;
smooth in vec2 txrCoords;
smooth in vec3 shadowCoords;

uniform sampler2D textureSampler;
uniform sampler2D shadowSampler;
out vec4 outputColor;


float computeShadow() {
  if(shadowCoords.z > 1.0)
    return 0.0;

  float bias = 0.001;
  float currentDepth = shadowCoords.z * 0.5 + 0.5;

  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadowSampler, 0) / 2;
  for(int x = -1; x <= 1; ++x) {
      for(int y = -1; y <= 1; ++y) {
          float pcfDepth = texture(shadowSampler, shadowCoords.xy * 0.5 + 0.5 + vec2(x, y) * texelSize).r;
          shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
      }
  }
  shadow /= 9.0;
  return shadow;
}

void main() {

  // Color
  vec3 colorized = vertexColor.xyz;
  outputColor = vec4(colorized, vertexColor.a);
  // Textures
  outputColor = texture(textureSampler, txrCoords);

  float occl = .7;
  outputColor *= 1.0 - (vertexOcclusion * vertexOcclusion / 9.0) * occl;

  float shadow = computeShadow();
  outputColor *= 1 - shadow * .5;
}
