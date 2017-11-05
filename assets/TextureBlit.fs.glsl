// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core

out vec4 FragColor;

in vec2 fsTexCoords;

uniform sampler2D TargetTexture;

void main()
{
    vec4 color = texture(TargetTexture, fsTexCoords);
    FragColor = vec4(color.rgb, 1.0);
}
