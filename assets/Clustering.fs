// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core

out vec4 FragColor;
in vec2 fsTexCoords;

uniform sampler2D TargetTexture;
uniform sampler2D PositionTexture;
uniform sampler2D NormalTexture;

void main()
{
    vec4 color = texture(TargetTexture, fsTexCoords);
    vec4 normal = texture(NormalTexture, fsTexCoords);

    if (color.a > 0.99)
    {
        // Empty space
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
    else if (color.a < 0.15)
    {
        // Surface
        //FragColor = vec4(0.4, 0.1, 0.1, 1.0);
        //FragColor = vec3(color.rgb, 1.0);
				FragColor = vec4(normal.rgb, 1.0);
    }
    else
    {
        // Arealight
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
