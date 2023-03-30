#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aTextureCoordinate;

out vec3 vColor;
out vec2 vTextureCoordinate;

uniform float uTime;

uniform mat3 uModelMatrix;

void main() {
    vColor = vec3(1,0,0);
    vTextureCoordinate = aTextureCoordinate;
    gl_Position = vec4((uModelMatrix*vec3(aVertexPosition, 1)).xy, 0, 1);
    
}