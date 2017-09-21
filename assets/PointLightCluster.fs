// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core

out vec4 FragColor;
in vec2 fsTexCoords;

uniform sampler2D TargetTexture;
uniform sampler2D PositionTexture;
uniform sampler2D NormalTexture;

uniform vec3 LightViewPosition;
uniform vec3 LightColor;
uniform vec3 LightViewNormal;
uniform float LightDistance;

uniform int NumPointLights;

const float pi = 3.14159265;

float distribution_ggx_tr(vec3 N, vec3 H, float a)
{
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotHSq = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotHSq * (a2 - 1.0) + 1);
    float pi = 3.1415926535897932384626433832795;
    denom = pi * denom * denom;

    return nom / denom;
}

vec3 fresnel_schlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float geometry_schlick_ggx(float NdotV, float k)
{
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

float geometry_smith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = geometry_schlick_ggx(NdotV, k);
    float ggx2 = geometry_schlick_ggx(NdotL, k);
    return ggx1 * ggx2;
}

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


vec3 shadeSurface(vec3 position, vec3 normal, vec3 albedo, float roughness)
{
    vec3 lightVec = LightViewPosition - position;
    vec3 lightDir = normalize(lightVec);
    vec3 viewDir = normalize(-position);
    float lightDist = length(lightVec);

    vec3 radiance = vec3(1,1,1) * attenuation(1, 0.7, 1.8, lightDist);

    float metalness = 0.0;
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
    //Lo /= NumPointLights;

    return vec3(Lo);
}

void main()
{
    vec4 gbufferA = texture(PositionTexture, fsTexCoords);
    vec4 gbufferB = texture(NormalTexture, fsTexCoords);
    vec4 gbufferC = texture(TargetTexture, fsTexCoords);

    float materialID = gbufferC.a;

    if (materialID > 0.99)
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    vec3 position = gbufferA.xyz;
    vec3 normal = normalize(gbufferB.xyz);
    vec3 albedo = gbufferC.rgb;
    float roughness = gbufferB.a;

    if (materialID < 0.15)
    {
        FragColor = vec4(shadeSurface(position, normal, albedo, roughness), 1.0);
    }
    else
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
