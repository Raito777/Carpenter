#version 330

in vec3 vWorldPos;

uniform vec3 uLightPos;
uniform float ufar_plane;

void main() {

    // get distance between fragment and point light source
    float lightDistance = distance(vWorldPos, uLightPos);

    lightDistance = lightDistance / ufar_plane;

    gl_FragDepth = lightDistance;
}