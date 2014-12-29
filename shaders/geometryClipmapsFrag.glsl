#version 400

out vec4 fragColour;

uniform sampler2D mudTexture;
uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D snowTexture;
uniform float mudHeight;
uniform float grassHeight;
uniform float rockHeight;
uniform sampler2D colourTexture;
uniform sampler2D geoTexture;
uniform sampler2D normalMap;
uniform sampler2D colourMap;
uniform mat4 normalMatrix;

uniform float fogMin;
uniform float fogMax;

uniform bool cutout;

in float height;
in vec2 texCoords;
in vec4 position;
in vec2 vertPos;

vec4 mudColour = texture(mudTexture,texCoords);
vec4 grassColour = texture(grassTexture,texCoords);
vec4 rockColour = texture(rockTexture, texCoords);
vec4 snowColour = texture(snowTexture,texCoords);

vec4 calcNormals(){
  const vec2 size = vec2(0.05,0.0);
  const ivec3 off = ivec3(-1,0,1);
  vec4 wave = texture(geoTexture, texCoords);
  float s11 = wave.x;
  float s01 = (textureOffset(geoTexture, texCoords, off.xy).x + textureOffset(geoTexture, texCoords, off.xy).y + textureOffset(geoTexture, texCoords, off.xy).z);
  float s21 = (textureOffset(geoTexture, texCoords, off.zy).x + textureOffset(geoTexture, texCoords, off.zy).y + textureOffset(geoTexture, texCoords, off.zy).z);
  float s10 = (textureOffset(geoTexture, texCoords, off.yx).x + textureOffset(geoTexture, texCoords, off.yx).y + textureOffset(geoTexture, texCoords, off.yx).z);
  float s12 = (textureOffset(geoTexture, texCoords, off.yz).x + textureOffset(geoTexture, texCoords, off.yz).y + textureOffset(geoTexture, texCoords, off.yz).z);
  vec3 va = normalize(vec3(size.xy,s21-s01));
  vec3 vb = normalize(vec3(size.yx,s12-s10));
  vec4 bump = vec4( cross(va,vb), s11 );
//  /bump = vec4(normalMatrix * bump);
  return bump;
}

vec4 calcColour(){

    vec4 finalColour;
    float mixThreshhold = 0.05;
        if(height<=mudHeight){
//            if(angle>30){
//                finalColour = mix(mudColour,rockColour,((angle-30.0)/60));
//            }
//            else{
                finalColour = mudColour;
//            }
            float x = mudHeight-height;
            if(x<=mixThreshhold){
                finalColour = mix(grassColour,finalColour,x/mixThreshhold);
            }
        }
        else if(height>mudHeight && height<=grassHeight){
//            if(angle>30){
//                finalColour = mix(grassColour,rockColour,((angle-30.0)/60));
//            }
//            else{
                finalColour = grassColour;
//            }
            float x = grassHeight-height;
            if(x<=mixThreshhold){
                finalColour = mix(rockColour,finalColour,x/mixThreshhold);
            }
        }
        else if(height>grassHeight && height<=rockHeight){
            float x = rockHeight-height;
            if(x<=mixThreshhold){
                finalColour = mix(snowColour,rockColour,x/mixThreshhold);
            }
            else{
                finalColour = rockColour;
            }
        }
        else if(height>rockHeight){
//            if(angle>30){
//                finalColour = mix(snowColour,rockColour,((angle-30.0)/60));
//            }
//            else{
                finalColour = snowColour;
//            }
        }
    return finalColour;
}

vec3 calcFog(vec3 texColour){
  float dist = length(position.xyz);
  vec3 rayDir = normalize(-position.xyz);
  vec3 sunDir = vec3(0.0, -0.3, 1.0);
  float fogAmount = exp( 1.0 -dist*1.0 );
  float sunAmount = max(dot( rayDir, sunDir ), 0.0 );
  vec3  fogColour  = mix(vec3(0.6, 0.7, 0.8), vec3(1.0,0.9, 0.7), pow(sunAmount,8.0) );

  float fogFactor = (fogMax - dist) / (fogMax - fogMin);
  fogFactor = clamp(fogFactor, 0.0, 1.0);
  //fogColour = vec3(1.0, 1.0, 1.0);
  return mix(fogColour, texColour, fogFactor);

}

void main(){
  vec3 texColour = texture(colourMap, texCoords).rgb;
//  vec3 texColour = texture(geoTexture, texCoords).rgb;
  vec3 colour = calcFog(texColour);
  fragColour = vec4(colour, 1.0);

  if (cutout){
    if (length(vertPos)<=0.05){
      discard;
    }
  }
}
