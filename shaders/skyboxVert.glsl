  #version 400
layout (location = 0) in vec3 vertexPosition;

out vec3 viewDir;
out vec3 position;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;

void main(){
    viewDir = vertexPosition;
    position = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));
    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition,1.0);

}
