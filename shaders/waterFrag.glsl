#version 400

out vec4 fragColour;
in vec4 position;

uniform float fogMin;
uniform float fogMax;

vec3 calcFog(vec3 texColour){
  float dist = length(position.xyz);
  vec3 rayDir = normalize(-position.xyz);
  vec3 sunDir = vec3(0.0, -0.3, 1.0);
  float fogAmount = exp( 1.0 -dist*1.0 );
  float sunAmount = max(dot( rayDir, sunDir ), 0.0 );
  vec3  fogColour  = mix(vec3(0.6, 0.7, 0.8), vec3(1.0,0.9, 0.7), pow(sunAmount,8.0) );

  float fogFactor = (fogMax - dist) / (fogMax - fogMin);
  fogFactor = clamp(fogFactor, 0.0, 1.0);
  //vec3 fogColour = vec3(1.0, 1.0, 1.0);
  return mix(fogColour, texColour, 1.0);

}
void main(){
  float dist = length(position.xyz);
  vec3 rayDir = normalize(-position.xyz);
  vec3 sunDir = vec3(0.0, -0.3, 1.0);
  float fogAmount = exp( 1.0 -dist*1.0 );
  float sunAmount = max(dot( rayDir, sunDir ), 0.0 );
  vec3  fogColour  = mix(vec3(0.6, 0.7, 0.8), vec3(1.0,0.9, 0.7), pow(sunAmount,8.0) );
  float fogFactor = (fogMax - dist) / (fogMax - fogMin);
  fogFactor = clamp(fogFactor, 0.0, 1.0);

  vec3 colour = mix(fogColour, vec3(0.0, 0.0, 1.0), fogFactor);
  fragColour = vec4(57.0/255.0, 84.0/255.0, 101.0/255.0, 1.0);

  if (fogFactor <= 0.3){
    discard;
  }


}
