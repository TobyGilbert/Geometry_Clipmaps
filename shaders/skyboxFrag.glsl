#version 400

uniform samplerCube cubeMapTex;
uniform mat3 normalMatrix;
uniform mat4 modelViewMatrix;

in vec3 viewDir;
in vec3 position;
out vec4 fragColour;

vec3 calcFog(vec3 texColour){
  float dist = length(position.xyz);
  vec3 rayDir = normalize(-position.xyz);
  vec3 sunDir = vec3(0.0, -0.3, 1.0);
  float fogAmount = exp(-dist*0.5 );
  float sunAmount = max(dot( rayDir, sunDir ), 0.0 );
  vec3  fogColour  = mix(vec3(0.6, 0.7, 0.8), vec3(1.0,0.9, 0.7), pow(sunAmount,10.0) );
  //return mix(fogColour, texColour, fogAmount);
  return fogColour;
}

void  main(){
   vec3 texColour = texture(cubeMapTex, viewDir).rgb;
   texColour = vec3(176.0/255.0, 226.0/255.0, 1.0);
   vec3 colour = calcFog(texColour);
   fragColour = vec4(colour, 1.0);
}
