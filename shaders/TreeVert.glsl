#version 400
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 offset;

out vec3 position;
out vec3 normal;
out vec2 TexCoords;
out float height;

//uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 rotateMatrix;
uniform mat4 modelViewProjectionMatrix;


void main(){
    TexCoords = texCoord;
    normal = normalize(normalMatrix*vertexNormal);
    normal = normal*-1;
    height = vertexPosition.y/7;
    mat4 translationMatrix = mat4(1.0);
    translationMatrix[3][0] = offset.x;
    translationMatrix[3][1] = offset.y;
    translationMatrix[3][2] = offset.z;
    position = vec3(modelViewMatrix * translationMatrix * vec4(vertexPosition.xyz, 1.0) );
    gl_Position = vec4(modelViewProjectionMatrix * translationMatrix *vec4(vertexPosition.xyz, 1.0));

}
