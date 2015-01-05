#version 400

vec3 LightDirection = vec3(0.0, -1.0, 0.0);
uniform vec3 viewPos;
//in vec3 viewPos;

out vec4 fragColour;

in vec2 TexCoords;
in vec4 pos;
in vec4 position;
in float time;

uniform sampler2D reflectMap;
uniform sampler2D refractMap;
//fog
uniform float fogMin;
uniform float fogMax;

uniform sampler2D normalMap;
uniform sampler2D dudv;

// Constants //
float kDistortion = 0.015;
float kReflection = 0.01;
vec4 baseColour = vec4(1.0, 1.0 ,1.0, 1.0);

vec4 tangent = vec4(1.0, 0.0, 0.0, 0.0);
vec4 lightNormal = vec4(0.0, 1.0, 0.0, 0.0);
vec4 biTangent = vec4(0.0, 0.0, 1.0, 0.0);

vec4 calcFog(vec4 texColour){
  float dist = length(position.xyz);
  vec3 rayDir = normalize(-position.xyz);
  vec3 sunDir = vec3(0.0, -0.3, 1.0);
  float fogAmount = exp( 1.0 -dist*1.0 );
  float sunAmount = max(dot( rayDir, sunDir ), 0.0 );
  vec3  fogColour  = mix(vec3(0.6, 0.7, 0.8), vec3(1.0,0.9, 0.7), pow(sunAmount,8.0) );

  float fogFactor = (fogMax - dist) / (fogMax - fogMin);
  fogFactor = clamp(fogFactor, 0.0, 1.0);
  return vec4(mix(fogColour, texColour.xyz, fogFactor), 0.81);

}
void main(){
  // Light tangent space
  vec4 lightDir = normalize(vec4(LightDirection.xyz, 1.0));
  vec4 lightTanSpace = normalize(vec4(dot(lightDir, tangent), dot(lightDir, biTangent), dot(lightDir, lightNormal), 1.0));

  // Fresnal Term
  vec4 distOffset = texture(dudv, TexCoords + vec2(time)) * kDistortion;
  vec4 normal = texture(normalMap, vec2(TexCoords + distOffset.xy));
  normal = normalize(normal * 2.0 - 1.0);
  normal.a = 0.81;

  vec4 lightReflection = normalize(reflect(-1 * lightTanSpace, normal));
  vec4 invertedFresnal = vec4(dot(normal, lightReflection));
  vec4 fresnalTerm = 1.0 - invertedFresnal;

  // Reflection
  vec4 dudvColour = texture(dudv, vec2(TexCoords + distOffset.xy));
  dudvColour = normalize(dudvColour * 2.0 -1.0) * kReflection;

  // Projection coordinates from http://www.bonzaisoftware.com/tnp/gl-water-tutorial/
  vec4 tmp = vec4(1.0/ pos.w);
  vec4 projCoord = pos * tmp;
  projCoord += vec4(1.0);
  projCoord *= vec4(0.5);
  projCoord += dudvColour;
  projCoord = clamp(projCoord, 0.001, 0.999);

  vec4 reflectionColour = mix(texture(reflectMap, projCoord.xy), baseColour, 0.3);
  reflectionColour *= fresnalTerm;

  // Refraction
  vec4 refractionColour = texture(refractMap, projCoord.xy);
  vec4 depthValue = vec4(0.1, 0.1, 0.1, 1.0);
  vec4 invDepth = 1.0 - depthValue;

  refractionColour *= invertedFresnal * invDepth;
  refractionColour += baseColour * depthValue * invertedFresnal;

  fragColour = calcFog(reflectionColour + refractionColour);
//  fragColour = texture(reflectMap, TexCoords);
//  fragColour = texture(refractMap, TexCoords);
}

