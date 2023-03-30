#version 330

in vec3 vColor;
in vec2 vTextureCoordinate;

out vec4 fFragColor;

uniform sampler2D uTexture;

void main() {
    
    fFragColor = texture(uTexture, vTextureCoordinate);
}