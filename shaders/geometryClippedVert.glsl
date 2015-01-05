#version 400
layout (location = 0) in vec3 vertexPosition;

uniform sampler2D geoTexture;

uniform vec4 map_position;
uniform vec4 offset;
uniform vec4 scale;
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 modelMatrix;
uniform float heightScale;
uniform mat4 normalMatrix;

out vec2 texCoords;
out float height;
out vec4 position;
out vec2 vertPos;
uniform bool reflect;

float gl_ClipDistance[1];

void main(){
   vec4 pos = scale * (offset+vec4(vertexPosition, 1.0));
   vec4 pos_map = ((pos+map_position) * 0.125)+0.5;
   pos.y = float(texture(geoTexture, pos_map.xz).r * texture(geoTexture, pos_map.xz).g + texture(geoTexture, pos_map.xz).b);
   height = pos.y/3.0;
   pos.y*=heightScale;
   texCoords = pos_map.xz;
   position =  vec4(pos.x, 1.0, pos.z, pos.w);
   vertPos = pos.xz;
   if (reflect){
     gl_ClipDistance[0] = dot(modelMatrix * pos, vec4(0.0, -1.0, 0.0, 0.0));
   }
   else{
     gl_ClipDistance[0] = dot(modelMatrix * pos, vec4(0.0, -1.0, 0.0, 0.0));
   }
   gl_Position = MVP * pos;
}
