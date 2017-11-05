// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

float distribution_ggx_tr(vec3 N, vec3 H, float a);
vec3 fresnel_schlick(float cosTheta, vec3 F0);
float geometry_schlick_ggx(float NdotV, float k);
float geometry_smith(vec3 N, vec3 V, vec3 L, float k);

float distribution_ggx_tr(vec3 N, vec3 H, float a)
{
    const float pi = 3.1415926535897932384626433832795;

    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotHSq = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotHSq * (a2 - 1.0) + 1);
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
