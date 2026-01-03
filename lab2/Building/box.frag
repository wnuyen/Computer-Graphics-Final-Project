#version 330 core

in vec3 color;
in vec2 UV;  // Must match vertex shader output exactly

uniform sampler2D textureSampler;

out vec3 finalColor;

void main()
{
    vec3 textureColor = texture(textureSampler, UV).rgb;
    finalColor = color * textureColor;
}
