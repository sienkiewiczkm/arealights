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

uniform int NumPointLights;

const float pi = 3.1415926535897932384626433832795;

float attenuation(
  float attConst,
  float attLinear,
  float attQuadratic,
  float lightDist
)
{
    float attenuationDenominator = (
        attConst + attLinear * lightDist + attQuadratic * lightDist * lightDist
    );

    return 1 / attenuationDenominator;
}


vec3 shadeSurface(vec3 position, vec3 normal, vec3 albedo, float metalness, float roughness)
{
    vec3 lightVec = LightViewPosition - position;
    vec3 lightDir = normalize(lightVec);
    vec3 viewDir = normalize(-position);
    float lightDist = length(lightVec);

    vec3 radiance = vec3(1,1,1) * attenuation(1, 0.7, 1.8, lightDist);

    vec3 F0 = mix(vec3(0.04), albedo, metalness);
    float NdotV = max(dot(normal, viewDir), 0.0);
    float NdotL = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(viewDir + lightDir);
    float D = distribution_ggx_tr(normal, halfwayDir, roughness);
    vec3 F = fresnel_schlick(max(dot(halfwayDir, viewDir), 0.0), F0);
    float G = geometry_smith(normal, viewDir, lightDir, roughness);

    vec3 BRDF = (D * F * G) / (4.0 * NdotV * NdotL + 0.001);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalness;

    vec3 Lo = (kD * albedo / pi + BRDF) * radiance * NdotL;

    float lightInfluence = clamp(dot(LightViewNormal, -lightDir), 0, 1);
    Lo *= lightInfluence;

    Lo /= NumPointLights;

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
