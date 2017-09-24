// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core

in vec3 fsPosition;
in vec3 fsNormal;
in vec3 fsTangent;
in vec2 fsTexCoord;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform float MaterialID;

uniform sampler2D AlbedoTexture;
uniform sampler2D NormalTexture;
uniform sampler2D MetalnessTexture;
uniform sampler2D RoughnessTexture;

float textureScale = 8.0f;

vec3 toLinear(vec3 color) {
  return pow(color, vec3(2.2));
}

void main()
{
		mat3 tbn = mat3(
			normalize(fsTangent),
			normalize(cross(fsTangent, fsNormal)),
			normalize(fsNormal)
		);

    vec2 texCoord = fsTexCoord * textureScale;
    vec3 tbnNormal = normalize(texture(NormalTexture, texCoord).rgb*2.0-1.0);
    vec3 albedo = toLinear(texture(AlbedoTexture, texCoord).rgb);
    float roughness = texture(RoughnessTexture, texCoord).r;

    gPosition = vec4(fsPosition, 0);
    gNormal = vec4(normalize(tbn * tbnNormal), roughness);
    gColor = vec4(albedo, MaterialID);
}
