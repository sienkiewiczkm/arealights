// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core

in vec3 fsPosition;
in vec3 fsNormal;
in vec2 fsTexCoord;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

float proceduralCheckerboardTexture(vec2 coordinate, int numCheckers)
{
    float checkerWidth = 1.0 / numCheckers;

    int xDivider = int(coordinate.x / checkerWidth);
    int yDivider = int(coordinate.y / checkerWidth);

    xDivider %= 2;
    yDivider %= 2;

    return (xDivider == yDivider) ? 1.0 : 0.0;
}

void main()
{
    vec3 firstColor = vec3(1.0, 1.0, 1.0);
    vec3 secondColor = vec3(1.0, 0.0, 1.0);
    vec3 finalColor = mix(
      firstColor,
      secondColor,
      proceduralCheckerboardTexture(fsTexCoord, 16)
    );

    gPosition = fsPosition;
    gNormal = fsNormal;
    gColor = vec4(finalColor, 1.0);
}
