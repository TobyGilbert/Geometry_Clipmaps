#version 400
layout (location = 0) in vec3 vertexPosition;

uniform sampler2D geoTexture;

uniform vec4 map_position;
uniform vec4 offset;
uniform vec4 scale;
uniform mat4 MVP;
uniform mat4 MV;
uniform float heightScale;

out vec2 texCoords;
out float height;
out vec4 position;
out vec2 vertPos;


void main(){
   vec4 pos = scale * (offset+vec4(vertexPosition, 1.0));
   vec4 pos_map = ((pos+map_position) * 0.125)+0.5;
   pos.y = float(texture(geoTexture, pos_map.xz).r * texture(geoTexture, pos_map.xz).g + texture(geoTexture, pos_map.xz).b);
   height = pos.y/3.0;
   pos.y*=heightScale;
   texCoords = pos_map.xz / 2.0 + 0.25;
   position =  MV * vec4(pos.x, 1.0, pos.z, pos.w);
   vertPos = pos.xz;
   gl_Position = MVP * pos;
}
