#version 430 core

precision mediump float;

smooth in vec3 vertexPosition;
smooth in vec3 vertexNormal;
smooth in float vertexOcclusion;
smooth in vec2 txrCoords;
smooth in vec2 normalCoords;
smooth in vec3 shadowCoords[3];
smooth in mat3 TBN;


uniform vec3 lightDirection;
uniform float lightIntensity;

uniform sampler2D t_color;
uniform sampler2D t_normal;
uniform sampler2D t_shadow[3];
uniform int underWater; //glUniform1b :(


out vec4 outputColor;

float computeShadow(int i) {
  vec3 normal = normalize(vertexNormal);
  float dotNormal = dot(normalize(lightDirection), normal);

  // float bias = 0.0;
  float bmin = 0.00035;
  float bmax = 0.0002;
  float bias = max(bmax * (1.0 - dotNormal), bmin);
  float currentDepth = shadowCoords[i].z * 0.5 + 0.5;
  vec2 texelSize = 1.0 / textureSize(t_shadow[i], 0) / 2;

  // no pcf
  // float pcfDepth = texture(t_shadow[i], shadowCoords[i].xy * 0.5 + 0.5).r;
  // return currentDepth - bias > pcfDepth ? 1.0 : 0.0;

  // pcf
  float shadow = 0.0;
  for(float x = -1.5; x <= 1.5; ++x) {
      for(float y = -1.5; y <= 1.5; ++y) {
          float pcfDepth = texture(t_shadow[i], shadowCoords[i].xy * 0.5 + 0.5 + vec2(x, y) * texelSize).r;
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

  vec3 normalizedLightDirection = normalize(lightDirection);

  vec3 normal = normalize(TBN * (texture(t_normal ,normalCoords).rgb *2.0 -1.0));
  float dotNormal = dot(normalizedLightDirection, normal);
  float lambertian = max(-dotNormal, 0.0);

  vec3 viewDir = normalize(-vertexPosition);
  // this is blinn phong

  vec3 halfDir =  normalize(-normalizedLightDirection + viewDir);
  float specAngle = max(dot(halfDir, normal), 0.0);
  float specular = pow(specAngle, 200);

  // Textures
  outputColor = texture(t_color, txrCoords);

  // shadow

  float shadow = 0.0;

  for (int i = 0 ; i < 3; i++) {
    if (texture(t_shadow[i], shadowCoords[i].xy * 0.5 + 0.5).r != 1.0) {
      shadow = 1 - computeShadow(i);
      break;
    }
  }

  vec4 color = outputColor;
  outputColor.xyz = color.xyz * .5;
  outputColor.xyz += color.xyz * lightIntensity * lambertian * shadow *.5 ;
  outputColor.xyz +=vec3(1.0f) * specular * shadow  * texture(t_normal ,normalCoords).a* 1.0;


  float occl = .7;
  outputColor.xyz *= 1.0 - (vertexOcclusion * vertexOcclusion / 9.0) * occl;

  if(underWater == 1){
    outputColor.rgb *=vec3(127.0f/255, 148.0f/255, 1.0f) ;
  }
  // show in which shadow cascade we are
// for (int i = 0 ; i < 3; i++) {
//    if (texture(t_shadow[i], shadowCoords[i].xy * 0.5 + 0.5).r != 1.0) {
//      outputColor[i] += 0.2;
//      break;
//    }
//  }

  if(outputColor.a < 0.1) {
    discard;
  }
}
