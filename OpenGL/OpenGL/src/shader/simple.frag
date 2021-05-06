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
out vec4 outputColor;

float computeShadow(float dotLightNormal) {
  // if(shadowCoords.z > 1.0)
  //   return 0.0;

  // float bias = 0.0002;
  // float bias = 0.001;

//  float bias=0.1 / textureSize(shadowSampler, 0).x;
//  bias =bias *dotLightNormal;
  float currentDepth0 = shadowCoords[0].z * 0.5 + 0.5;
  float currentDepth1 = shadowCoords[0].z * 0.5 + 0.5;
  float currentDepth2 = shadowCoords[0].z * 0.5 + 0.5;
//vec2 texelSize = 1.0 / textureSize(shadowSampler, 0) / 2;

  // no pcf
  // float pcfDepth = texture(shadowSampler, shadowCoords.xy * 0.5 + 0.5).r;
  // return currentDepth - bias > pcfDepth ? 1.0 : 0.0;

  // pcf
//  float shadow = 0.0;
//  for(float x = -1.5; x <= 1.5; ++x) {
//      for(float y = -1.5; y <= 1.5; ++y) {
//          float pcfDepth = texture(shadowSampler, shadowCoords.xy * 0.5 + 0.5 + vec2(x, y) * texelSize).r;
//          shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
//      }
//  }
//  shadow /= 16.0;

 float pcfDepth0 = texture(shadowSampler[0], shadowCoords[0].xy * 0.5 + 0.5).r;
 float pcfDepth1 = texture(shadowSampler[1], shadowCoords[1].xy * 0.5 + 0.5).r;
 float pcfDepth2 = texture(shadowSampler[2], shadowCoords[2].xy * 0.5 + 0.5).r;
 if(currentDepth0 > pcfDepth0) return 1.0;
 if(currentDepth1 > pcfDepth1) return 1.0;
 return currentDepth2 > pcfDepth2 ? 1.0 : 0.0;
}

void main() {
  //vec3 normal = normalize(-vertexNormal);
  //float lambertian = max(dot(normalize(lightDirection), normal), 0.0);

  // Textures
  outputColor = texture(textureSampler, txrCoords);
  float shadow = 1 - computeShadow(dot(lightDirection,normalize(-vertexNormal)));
  outputColor = outputColor * .5 + outputColor * lightIntensity * shadow * .5;

  float occl = .7;
  outputColor *= 1.0 - (vertexOcclusion * vertexOcclusion / 9.0) * occl;

  outputColor = texture(textureSampler, (gl_FragCoord.xy*0.5+0.5)/1024.0);
  outputColor = texture(shadowSampler[1], (gl_FragCoord.xy*0.5+0.5)/400.0);
}
