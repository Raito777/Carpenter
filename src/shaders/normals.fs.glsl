#version 330

//variables d'entrées
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fFragColor;

void main() {
    fFragColor = vec4(normalize(vNormal_vs),1);
};