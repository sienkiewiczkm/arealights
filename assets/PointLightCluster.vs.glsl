// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoords;

out vec2 fsTexCoords;

void main()
{
    gl_Position = vec4(inPosition, 1.0);
    fsTexCoords = inTexCoords;
}

