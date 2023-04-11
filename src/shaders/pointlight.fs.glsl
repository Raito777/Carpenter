#version 330

//variables d'entrées
in vec4 vLightSpacePos;
in vec3 vPosition_vs; //w0 normalize(-vPos)
in vec3 vNormal_vs;
in vec2 vTexCoords;
in vec3 vWorldPos;

out vec4 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uKa_Light1;

uniform vec3 uLightPos_vs_Light1;
uniform vec3 uLightIntensity_Light1; //Li

uniform vec3 uKa_Light2;
uniform vec3 uLightPos_vs_Light2;
uniform vec3 uLightDir_vs_Light2;
uniform vec3 uLightIntensity_Light2; //Li


uniform sampler2D uTextureImg;


uniform float ufar_plane;

uniform samplerCube uTexture;


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

float calcShadowFactorPointLight() {
    vec3 LightToVertex = vWorldPos - uLightPos_vs_Light1;
    
    float Distance = length(LightToVertex)/ufar_plane;

    float SampledDistance = texture(uTexture, LightToVertex).r;

    float bias = 0.025;

    if ((SampledDistance + bias) < Distance) {
        return 0.15;
    }
    else {
        return 1.0;
    }
}

void main() {
    vec4 texColor = texture(uTextureImg, vTexCoords);
    vec4 lightColor = vec4(uKa_Light1 + calcShadowFactorPointLight()*(pointBlinnPhong()+dirblinnPhong()), 1);
    fFragColor = mix(texColor, lightColor, 0.5); // 0.5 est le coefficient d'interpolation, ajustez-le en fonction de vos besoins
}