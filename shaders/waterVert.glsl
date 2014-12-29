#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 texCoord;

uniform mat4 MVP;
out vec4 position;

uniform mat4 modelViewMatrix;

void main(){
  position = vec4(8.0, 1.0, 8.0, 1.0) *vec4(vertexPosition.x, 1.0, vertexPosition.z, 1.0);
  gl_Position = MVP * vec4(vertexPosition, 1.0);
}
