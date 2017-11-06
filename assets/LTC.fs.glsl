// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>
//
// Lots of code here comes directly from:
//  Real-Time Polygonal-Light Shading with Linearly Transformed Cosines.
//  Eric Heitz, Jonathan Dupuy, Stephen Hill and David Neubelt.
//  ACM Transactions on Graphics (Proceedings of ACM SIGGRAPH 2016) 35(4), 2016.
//  Project page: https://eheitzresearch.wordpress.com/415-2/

#version 330 core
#extension GL_ARB_shading_language_include : require

#include "/gbuffer.glsl"

out vec4 FragColor;

in vec2 fsTexCoords;

uniform mat4 viewMatrix;

uniform sampler2D LTCLookupA;
uniform sampler2D LTCLookupB;

uniform mat4 arealightTransform;

const float LUT_SIZE = 64.0;
const float LUT_SCALE = (LUT_SIZE - 1.0)/LUT_SIZE;
const float LUT_BIAS = 0.5/LUT_SIZE;

const float pi = 3.1415926535897932384626433832795;

float IntegrateEdge(vec3 v1, vec3 v2)
{
    float x = dot(v1, v2);
    float y = abs(x);

    float a = 0.8543985 + (0.4965155 + 0.0145206*y)*y;
    float b = 3.4175940 + (4.1616724 + y)*y;
    float v = a / b;

    float theta_sintheta = (x > 0.0) ? v : 0.5*inversesqrt(1.0 - x*x) - v;

    return (cross(v1, v2)*theta_sintheta).z;
}

void ClipQuadToHorizon(inout vec3 L[5], out int n)
{
    // detect clipping config
    int config = 0;
    if (L[0].z > 0.0) config += 1;
    if (L[1].z > 0.0) config += 2;
    if (L[2].z > 0.0) config += 4;
    if (L[3].z > 0.0) config += 8;

    // clip
    n = 0;

    if (config == 0)
    {
        // clip all
    }
    else if (config == 1) // V1 clip V2 V3 V4
    {
        n = 3;
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
        L[2] = -L[3].z * L[0] + L[0].z * L[3];
    }
    else if (config == 2) // V2 clip V1 V3 V4
    {
        n = 3;
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
    }
    else if (config == 3) // V1 V2 clip V3 V4
    {
        n = 4;
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
        L[3] = -L[3].z * L[0] + L[0].z * L[3];
    }
    else if (config == 4) // V3 clip V1 V2 V4
    {
        n = 3;
        L[0] = -L[3].z * L[2] + L[2].z * L[3];
        L[1] = -L[1].z * L[2] + L[2].z * L[1];
    }
    else if (config == 5) // V1 V3 clip V2 V4) impossible
    {
        n = 0;
    }
    else if (config == 6) // V2 V3 clip V1 V4
    {
        n = 4;
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
        L[3] = -L[3].z * L[2] + L[2].z * L[3];
    }
    else if (config == 7) // V1 V2 V3 clip V4
    {
        n = 5;
        L[4] = -L[3].z * L[0] + L[0].z * L[3];
        L[3] = -L[3].z * L[2] + L[2].z * L[3];
    }
    else if (config == 8) // V4 clip V1 V2 V3
    {
        n = 3;
        L[0] = -L[0].z * L[3] + L[3].z * L[0];
        L[1] = -L[2].z * L[3] + L[3].z * L[2];
        L[2] =  L[3];
    }
    else if (config == 9) // V1 V4 clip V2 V3
    {
        n = 4;
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
        L[2] = -L[2].z * L[3] + L[3].z * L[2];
    }
    else if (config == 10) // V2 V4 clip V1 V3) impossible
    {
        n = 0;
    }
    else if (config == 11) // V1 V2 V4 clip V3
    {
        n = 5;
        L[4] = L[3];
        L[3] = -L[2].z * L[3] + L[3].z * L[2];
        L[2] = -L[2].z * L[1] + L[1].z * L[2];
    }
    else if (config == 12) // V3 V4 clip V1 V2
    {
        n = 4;
        L[1] = -L[1].z * L[2] + L[2].z * L[1];
        L[0] = -L[0].z * L[3] + L[3].z * L[0];
    }
    else if (config == 13) // V1 V3 V4 clip V2
    {
        n = 5;
        L[4] = L[3];
        L[3] = L[2];
        L[2] = -L[1].z * L[2] + L[2].z * L[1];
        L[1] = -L[1].z * L[0] + L[0].z * L[1];
    }
    else if (config == 14) // V2 V3 V4 clip V1
    {
        n = 5;
        L[4] = -L[0].z * L[3] + L[3].z * L[0];
        L[0] = -L[0].z * L[1] + L[1].z * L[0];
    }
    else if (config == 15) // V1 V2 V3 V4
    {
        n = 4;
    }

    if (n == 3)
        L[3] = L[0];
    if (n == 4)
        L[4] = L[0];
}

vec3 LTC_Evaluate(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 points[4], bool twoSided)
{
    // construct orthonormal basis around N
    vec3 T1, T2;
    T1 = normalize(V - N*dot(V, N));
    T2 = cross(N, T1);

    // rotate area light in (T1, T2, N) basis
    Minv = Minv * transpose(mat3(T1, T2, N));

    // polygon (allocate 5 vertices for clipping)
    vec3 L[5];
    L[0] = Minv * (points[0] - P);
    L[1] = Minv * (points[1] - P);
    L[2] = Minv * (points[2] - P);
    L[3] = Minv * (points[3] - P);

    int n;
    ClipQuadToHorizon(L, n);

    if (n == 0)
        return vec3(0, 0, 0);

    L[0] = normalize(L[0]);
    L[1] = normalize(L[1]);
    L[2] = normalize(L[2]);
    L[3] = normalize(L[3]);
    L[4] = normalize(L[4]);

    float sum = 0.0;
    sum += IntegrateEdge(L[0], L[1]);
    sum += IntegrateEdge(L[1], L[2]);
    sum += IntegrateEdge(L[2], L[3]);

    if (n >= 4)
        sum += IntegrateEdge(L[3], L[4]);
    if (n == 5)
        sum += IntegrateEdge(L[4], L[0]);

    sum = twoSided ? abs(sum) : max(0.0, sum);

    vec3 Lo_i = vec3(sum);

    return Lo_i;
}

vec3 toLinear(vec3 color) {
  return pow(color, vec3(2.2));
}

vec3 shadeSurface(vec3 position, vec3 normal, vec3 albedo, float metalness, float roughness)
{
  vec3 points[4];
  float hw = 0.5*1.0;
  float hh = 0.5*1.0;
  points[0] = (viewMatrix * arealightTransform * vec4(-hw, -hh, 0, 1.0)).xyz;
  points[1] = (viewMatrix * arealightTransform * vec4(-hw, +hh, 0, 1.0)).xyz;
  points[2] = (viewMatrix * arealightTransform * vec4(hw, +hh, 0, 1.0)).xyz;
  points[3] = (viewMatrix * arealightTransform * vec4(hw, -hh, 0, 1.0)).xyz;

  vec3 viewDir = normalize(-position);

  float ndotv = clamp(dot(normal, viewDir), 0, 1);
  vec2 uv = vec2(roughness, sqrt(1 - ndotv));
  uv = uv*LUT_SCALE + LUT_BIAS;

  vec4 ltcLookupA = texture(LTCLookupA, uv).rgba;
  vec3 ltcLookupB = texture(LTCLookupB, uv).rgb;
  float ltcMagnitude = ltcLookupB.y;
  float ltcFresnel = ltcLookupB.z;

  mat3 Minv = mat3(
      vec3(ltcLookupA.x, 0,            ltcLookupA.y),
      vec3(0,            ltcLookupA.z, 0),
      vec3(ltcLookupA.w, 0,            ltcLookupB.x)
  );

  vec3 diff = LTC_Evaluate(normal, viewDir, position, mat3(1), points, false);

  vec3 specularColor = vec3(1);
  vec3 lightColor = vec3(1);

  vec3 spec = LTC_Evaluate(normal, viewDir, position, Minv, points, false);
  spec *= specularColor * ltcMagnitude + (1 - specularColor) * ltcFresnel;

  vec3 color = lightColor * (spec*metalness + (1-metalness) * albedo * diff);

  return color;
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
        vec3 surfaceColor = shadeSurface(
            gbuffer.position,
            gbuffer.normal,
            gbuffer.albedo,
            gbuffer.metalness,
            gbuffer.roughness
        );

        FragColor = vec4(surfaceColor, 1.0);
    }
    else
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
