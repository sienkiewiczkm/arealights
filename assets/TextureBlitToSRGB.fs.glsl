// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#version 330 core

out vec4 FragColor;

in vec2 fsTexCoords;

uniform sampler2D TargetTexture;

vec3 toSRGB(vec3 color) {
  return pow(color, vec3(1.0/2.2));
}

void main()
{
    vec4 color = texture(TargetTexture, fsTexCoords);
    FragColor = vec4(toSRGB(color.rgb), 1.0);
}
