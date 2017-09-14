// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 fsPosition;
out vec3 fsNormal;
out vec2 fsTexCoord;

void main()
{
    vec4 viewPosition = viewMatrix * modelMatrix * vec4(inPosition.xyz, 1.0);
    gl_Position = projectionMatrix * viewPosition;

    mat3 normalMatrix = mat3(viewMatrix) * transpose(inverse(mat3(modelMatrix)));

    fsPosition = viewPosition.xyz;
    fsNormal = normalize(normalMatrix * inNormal);
    fsTexCoord = inTexCoord;
}
