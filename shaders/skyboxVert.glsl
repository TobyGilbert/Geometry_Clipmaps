  #version 400
layout (location = 0) in vec3 vertexPosition;

out vec3 viewDir;

uniform mat4 modelViewProjectionMatrix;

void main(){
    viewDir = vertexPosition;
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition,1.0);

}
