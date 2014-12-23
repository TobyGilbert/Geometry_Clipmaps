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


struct fogInfo{
   float maxDistance;
   float minDistance;
   vec3 colour;
};

uniform fogInfo fog;
uniform bool cutout;

in float height;
in vec2 texCoords;
in vec4 position;
in vec2 vertPos;

vec4 mudColour = texture(mudTexture,texCoords);
vec4 grassColour = texture(grassTexture,texCoords);
vec4 rockColour = texture(rockTexture, texCoords);
vec4 snowColour = texture(snowTexture,texCoords);


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

void main(){
  float dist = length(position.xyz);
  float fogFactor = (fog.maxDistance - dist) / (fog.maxDistance - fog.minDistance);
  fogFactor = clamp(fogFactor, 0.0, 1.0);
  vec3 texColour = /* calcColour().rgb;/*/texture(colourMap, texCoords).rgb;// vec3(0.9, 0.1, 0.1);
  vec3 colour = mix(fog.colour, texColour, fogFactor);

  fragColour =texture(colourMap, texCoords);
//  fragColour = vec4(colour, 1.0);
//  fragColour = calcColour();
  //fragColour = vec4(height, height, height, 1.0);
  //fragColour = texture(grassTexture, texCoords);

  if (cutout){
    if (length(vertPos)<=0.05){
      discard;
    }
  }
}
