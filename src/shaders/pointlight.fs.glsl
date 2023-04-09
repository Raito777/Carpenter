#version 330

//variables d'entrées
in vec3 vPosition_vs; //w0 normalize(-vPos)
in vec3 vNormal_vs;
in vec2 vTexCoords;

in vec4 vLightSpacePos;


out vec4 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uKa_Light1;
uniform vec3 uLightPos_vs_Light1 = vec3(0.0, 0.0, 0.0); //wi 
uniform vec3 uLightIntensity_Light1  = vec3(0.0, 0.0, 0.0); //Li

uniform vec3 uLightDir_vs_Light2 = vec3(0.0, 0.0, 1.0);; //wi 
uniform vec3 uLightIntensity_Light2  = vec3(0.0, 0.0, 0.0); ; //Li

uniform sampler2D uTexture;



vec3 pointBlinnPhong() {
        float d = distance(vPosition_vs, uLightPos_vs_Light1);
        vec3 Li = (uLightIntensity_Light1 / (d * d));
        vec3 N = vNormal_vs;
        vec3 w0 = normalize(-vPosition_vs);
        vec3 wi = normalize(uLightPos_vs_Light1 - vPosition_vs);
        vec3 halfVector = (w0 + wi)/2.f;
        
        return uKa_Light1 + Li*(uKd*max(dot(wi, N), 0.) + uKs*pow(max(dot(halfVector, N), 0.), uShininess));
}

vec3 dirblinnPhong() {
        vec3 Li = uLightIntensity_Light2;
        vec3 N = normalize(vNormal_vs);
        vec3 w0 = normalize(-vPosition_vs);
        vec3 wi = normalize(uLightDir_vs_Light2);
        vec3 halfVector = (w0 + wi)/2.f;
        
        return Li*(uKd*max(dot(wi, N), 0.) + uKs*max(pow(dot(halfVector, N), 0.), uShininess));
}

float calcShadowFactor() {
    vec3 projCoords = vLightSpacePos.xyz / vLightSpacePos.w;

    if(projCoords.z > 1.0) {
        return 1.0;
    }

    vec2 UVCoords = vec2(0.5 * projCoords.x + 0.5, 0.5 * projCoords.y + 0.5);
    float z = 0.5 * projCoords.z + 0.5;

    float depth = texture(uTexture, UVCoords).x;

    float DiffuseFactor = dot(vNormal_vs, -uLightDir_vs_Light2);
    float bias = mix(0.01, 0.0, DiffuseFactor);

    if (depth + bias < z)
        return 0.5;
    else
        return 1.0;
}


vec3 projCoords = vLightSpacePos.xyz / vLightSpacePos.w;
vec2 UVCoords = vec2(0.5 * projCoords.x + 0.5, 0.5 * projCoords.y + 0.5);
float z = 0.5 * projCoords.z + 0.5;
float depth = texture(uTexture, UVCoords).x;

void main() {
    fFragColor = vec4(uKa_Light1 + calcShadowFactor()*(pointBlinnPhong()+dirblinnPhong()), 1);
}