#version 300 es

precision mediump float;

#define UNDERWATER_FLAG 1
#define TRANSPARENT_FLAG 2

in vec3 vertexPosition;
in vec3 vertexNormal;
in float vertexOcclusion;
in vec2 texCoords;
in vec2 normalCoords;
in vec3 shadowCoords[3];
in mat3 TBN;


uniform vec3 lightDirection;
uniform float lightIntensity;
uniform float sunAmount;

uniform sampler2D t_color;
uniform sampler2D t_normal;
uniform sampler2D t_shadow[3];
uniform int flags;


out vec4 outputColor;

float linearizeDepth(float depth) { // https://learnopengl.com/Advanced-OpenGL/Depth-testing
    float near = 0.1;
    float far = 200.0;
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near)) / far;
}

void main() {

  vec3 normalizedLightDirection = normalize(lightDirection);

  vec3 normal = normalize(TBN * (texture(t_normal, normalCoords).rgb * 2.0 - 1.0));
  float dotNormal = dot(normalizedLightDirection, normal);
  float lambertian = max(-dotNormal, 0.0);

  vec3 viewDir = normalize(-vertexPosition);
  // this is blinn phong

  vec3 halfDir =  normalize(-normalizedLightDirection + viewDir);
  float specAngle = max(dot(halfDir, normal), 0.0);
  float specular = pow(specAngle, 200.0);

  // Textures
  outputColor = texture(t_color, texCoords);

  // shadow
  float shadow = 1.0;

  vec4 color = outputColor;
  outputColor.xyz = color.xyz * (0.2 + 0.4 * sunAmount);
  outputColor.xyz += color.xyz * lightIntensity * lambertian * shadow * 0.4 * sunAmount;
  outputColor.xyz += vec3(1.0) * specular * shadow * texture(t_normal, normalCoords).a * 1.0 * sunAmount;

  float occl = .7;
  outputColor.xyz *= 1.0 - (vertexOcclusion * vertexOcclusion / 9.0) * occl;

  if((flags & TRANSPARENT_FLAG) == 0) { // not transparent
    if(outputColor.a < 0.5) discard;
    outputColor /= outputColor.a;
  }
  if((flags & UNDERWATER_FLAG) == 1) { // is underwater
    outputColor.rgb *= vec3(127.0 / 255.0, 148.0 / 255.0, 1.0) ;
  }
}
