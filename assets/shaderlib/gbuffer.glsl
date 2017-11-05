// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

uniform sampler2D GBufferA; // position
uniform sampler2D GBufferB; // normal
uniform sampler2D GBufferC; // target

struct GBufferPackedData {
    vec4 a;
    vec4 b;
    vec4 c;
};

struct GBufferData {
    vec3 position;
    vec3 normal;
    vec3 albedo;
    float roughness;
    float metalness;
    float materialID;
};

GBufferData getGbufferData(vec2 fsTexCoords) {
    vec4 gbufferA = texture(GBufferA, fsTexCoords);
    vec4 gbufferB = texture(GBufferB, fsTexCoords);
    vec4 gbufferC = texture(GBufferC, fsTexCoords);

    GBufferData data;
    data.position = gbufferA.xyz;
    data.normal = normalize(gbufferB.xyz);
    data.albedo = gbufferC.rgb;
    data.roughness = gbufferB.a;
    data.metalness = gbufferA.a;
    data.materialID = gbufferC.a;

    return data;
}

GBufferPackedData packGbuffer(GBufferData gbuffer) {
    GBufferPackedData data;
    data.a = vec4(gbuffer.position, gbuffer.metalness);
    data.b = vec4(gbuffer.normal, gbuffer.roughness);
    data.c = vec4(gbuffer.albedo, gbuffer.materialID);
    return data;
}
