// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core
#extension GL_ARB_shading_language_include : require

#include "/gbuffer.glsl"

out vec4 FragColor;
in vec2 fsTexCoords;

void main()
{
    GBufferData gbuffer = getGbufferData(fsTexCoords);

    if (gbuffer.materialID > 0.99)
    {
        // Empty space
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
    else if (gbuffer.materialID < 0.15)
    {
        // Surface
        FragColor = vec4(gbuffer.albedo.rgb, 1.0);
    }
    else
    {
        // Arealight
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
