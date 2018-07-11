// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core
#extension GL_ARB_shading_language_include : require

#include "/gbuffer.glsl"
#include "/cookTorrance.glsl"

out vec4 FragColor;
in vec2 fsTexCoords;

uniform vec3 LightViewPosition;
uniform vec3 LightColor;
uniform vec3 LightViewNormal;
uniform float LightDistance;
uniform float LightFlux;

uniform int NumPointLights;

const float pi = 3.1415926535897932384626433832795;

vec3 shadeSurface(vec3 position, vec3 normal, vec3 albedo, float metalness, float roughness)
{
    vec3 lightVec = LightViewPosition - position;
    vec3 lightDir = normalize(lightVec);
    vec3 viewDir = normalize(-position);

    vec3 intensity = vec3(LightFlux / NumPointLights / (2 * pi * pi));

    vec3 F0 = mix(vec3(0.04), albedo, metalness);
    vec3 halfwayDir = normalize(viewDir + lightDir);
    float D = distribution_ggx_tr(normal, halfwayDir, roughness);
    vec3 F = fresnel_schlick(max(dot(halfwayDir, viewDir), 0.0), F0);
    float G = geometry_smith(normal, viewDir, lightDir, roughness*0.5);

    float NdotV = max(dot(halfwayDir, viewDir), 0.0);
    float NdotL = max(dot(halfwayDir, lightDir), 0.0);

    vec3 BRDF = (D * F * G) / (4.0 * NdotV * NdotL + 0.001);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalness;

    vec3 Lo = (kD * albedo / pi + BRDF) * intensity * NdotL;

    if (dot(LightViewNormal, -lightDir) < 0) {
        Lo *= 0;
    }

    // Fall-off is required but I am not sure what coefficient to use.
    float lightDist = length(lightVec);
    float falloff = 1 / (lightDist*lightDist);
    Lo *= falloff;

    return vec3(Lo);
}

void main()
{
    GBufferData gbuffer = getGbufferData(fsTexCoords);

    if (gbuffer.materialID > 0.99)
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    if (gbuffer.materialID < 0.15)
    {
        FragColor = vec4(
            shadeSurface(
                gbuffer.position,
                gbuffer.normal,
                gbuffer.albedo,
                gbuffer.metalness,
                gbuffer.roughness
            ),
            1.0
        );
    }
    else
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
