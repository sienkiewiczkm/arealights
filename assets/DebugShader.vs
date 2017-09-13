// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;

uniform mat4 worldMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;

out vec2 outTexCoord;

void main()
{
    mat4 viewProj = projMatrix * viewMatrix;
    gl_Position = viewProj * vec4(inPosition.xyz, 1.0);
    outTexCoord = inTexCoord;
}
