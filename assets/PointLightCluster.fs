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

vec3 shadeSurface(vec3 position, vec3 normal, vec4 color)
{
    vec3 lightVec = LightViewPosition - position;
    vec3 lightDir = normalize(lightVec);
    float lightDist = length(lightVec);

    float diffuse = max(0, dot(normal, lightDir));

    float attConst = 1.0;
    float attLinear = 0.7;
    float attQuadratic = 1.8;

    float attenuationDenominator = (
        attConst + attLinear * lightDist + attQuadratic * lightDist * lightDist
    );

    float attenuation = 1 / attenuationDenominator;

    float lightInfluence = clamp(10 * dot(LightViewNormal, -lightDir), 0, 1);

    diffuse *= lightInfluence;
    diffuse *= attenuation;
    diffuse /= NumPointLights;

    return vec3(diffuse);
}

void main()
{
    vec4 color = texture(TargetTexture, fsTexCoords);

    if (color.a > 0.99)
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    vec3 position = texture(PositionTexture, fsTexCoords).xyz;
    vec3 normal = normalize(texture(NormalTexture, fsTexCoords).xyz);

    if (color.a < 0.15)
    {
        FragColor = vec4(shadeSurface(position, normal, color), 1.0);
    }
    else
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
