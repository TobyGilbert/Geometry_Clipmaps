#version 400

uniform samplerCube cubeMapTex;

in vec3 viewDir;
out vec4 fragColour;

void  main(){
    vec3 fogColour = vec3(0.898 ,0.980, 1.0);
    vec3 texColour = texture(cubeMapTex, viewDir).rgb;
    float fogFactor = 0.0;
   fragColour = vec4(mix(fogColour, texColour, fogFactor), 1.0);
}
