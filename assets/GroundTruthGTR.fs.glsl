// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core
#extension GL_ARB_shading_language_include : require

#include "/gbuffer.glsl"
#include "/cookTorrance.glsl"

#define NUM_SAMPLES_PER_FRAME 128

out vec4 FragColor;
in vec2 fsTexCoords;

uniform vec3 LightColor;
uniform vec3 RectangularArealight[4];
uniform float LightDistance;
uniform vec2 RandomParameters[NUM_SAMPLES_PER_FRAME];

const float pi = 3.1415926535897932384626433832795;

bool quad_ray_intersection(vec3 q[4], vec3 pos, vec3 dir, out vec2 uv)
{
    // Create quad frame in one of the corners, we pick 0 for that purpose
    vec3 xaxis = q[1].xyz - q[0].xyz;
    vec3 yaxis = q[3].xyz - q[0].xyz;

    // Get width and height of the rectangle and normalize axes
    float xlen = length(xaxis);
    float ylen = length(yaxis);
    xaxis = xaxis / xlen;
    yaxis = yaxis / ylen;

    vec3 zaxis = -normalize(cross(xaxis, yaxis));

    // If looking direction and arealight "direction" is perpendicular we assume there is no intersection,
    // instead of taking line on rectangle.
    float ndotz = dot(dir, zaxis);
    if (ndotz < 0.00001) {
        return false;
    }

    // Compute intersection point using equation system:
    //
    //   I - intersection point
    //   zaxis - rectangle normal, defined as above
    //
    //   1. Intersection point must lie on a plane created by the rectangle:
    //        dot(zaxis, I - q[0]) = 0
    //   2. Intersection point must lie on a ray defined by pos and dir:
    //        I = pos + dir * t
    //
    //   Simple subsitution leads to:
    //     dot(zaxis, pos + dir * t - q[0]) =
    //       = dot(zaxis, pos - q[0]) + t * dot(zaxis, dir) = 0
    //

    float t = -(dot(zaxis, pos - q[0])) / ndotz;

    // Eliminate backward solutions
    if (t < 0.0) {
        return false;
    }

    vec3 intersection_pt = pos + dir * t;

    // Use intersection point to find texture coordinate on the rectangular arealight
    uv = vec2(
            dot(xaxis, intersection_pt - q[0].xyz),
            dot(yaxis, intersection_pt - q[0].xyz)
        ) / vec2(xlen, ylen);

    // Check if we are indeed within the boundaries (or not just anywhere on the plane).
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        return false;
    }

    return true;
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

vec3 shadeSurface(vec3 position, vec3 normal, vec3 albedo, float metalness, float roughness, vec2 randomParameters)
{
    float phi = randomParameters.x * (2.0 * pi);
    float theta = randomParameters.y * (0.5 * pi);
    vec3 tangentSample = vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));

    // TODO: Make "good" frame here
    vec3 localX = vec3(1.0, 0.0, 0.0);
    vec3 localZ = cross(localX, normal);
    localX = cross(normal, localZ);

    vec3 viewSample = normalize(localX * tangentSample.x + localZ * tangentSample.y + normal * tangentSample.z);

    vec2 uv;

    if (!quad_ray_intersection(RectangularArealight, position, viewSample, uv))
    {
        return vec3(0);
    }

    vec3 posOnEdgeA = mix(RectangularArealight[0], RectangularArealight[1], uv.x);
    vec3 posOnEdgeB = mix(RectangularArealight[3], RectangularArealight[2], uv.x);
    vec3 lightPos = mix(posOnEdgeA, posOnEdgeB, uv.y);

    vec3 lightVec = lightPos - position;
    vec3 lightDir = normalize(lightVec);
    vec3 viewDir = normalize(-position);
    float lightDist = length(lightVec);

    vec3 radiance = vec3(1,1,1);

    vec3 F0 = mix(vec3(0.04), albedo, metalness);
    float NdotV = max(dot(normal, viewDir), 0.0);
    float NdotL = max(dot(normal, lightDir), 0.0);

    float alpha = roughness * roughness;

    vec3 halfwayDir = normalize(viewDir + lightDir);
    float D = distribution_ggx_tr(normal, halfwayDir, alpha);
    vec3 F = fresnel_schlick(max(dot(halfwayDir, viewDir), 0.0), F0);
    float G = geometry_smith(normal, viewDir, lightDir, alpha);

    vec3 BRDF = (D * F * G) / (4.0 * NdotV * NdotL + 0.001);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metalness;

    vec3 Lo = (kD * albedo / pi + BRDF) * radiance * NdotL;

    Lo *= cos(theta) * sin(theta);

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
        vec3 accumulator = vec3(0);
        for (int i = 0; i < NUM_SAMPLES_PER_FRAME; ++i) {
            vec2 randomParameters = RandomParameters[i];
            accumulator += shadeSurface(
                gbuffer.position,
                gbuffer.normal,
                gbuffer.albedo,
                gbuffer.metalness,
                gbuffer.roughness,
                randomParameters
            );
        }

        // TODO: Remove magnification of result. There is something wrong.
        accumulator *= 100.0f;

        FragColor = vec4(accumulator / NUM_SAMPLES_PER_FRAME, 1.0);
    }
    else
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
