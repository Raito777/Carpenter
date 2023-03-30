#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec4 vPosition;
out vec4 vNormal;
out vec2 vTextureCoordinate;

void main() {
    vPosition = uMVMatrix * vec4(aPosition,1);
    vNormal = uNormalMatrix * vec4(aNormal,0);
    vTextureCoordinate = aTexCoords;
    gl_Position = uMVPMatrix * vec4(aPosition,1);
}