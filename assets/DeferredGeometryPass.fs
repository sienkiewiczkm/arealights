// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core

in vec3 fsPosition;
in vec3 fsNormal;
in vec2 fsTexCoord;

uniform float MaterialID;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

void main()
{
    gPosition = fsPosition;
    gNormal = normalize(fsNormal);
    gColor = vec4(1.0, 1.0, 1.0, MaterialID);
}
