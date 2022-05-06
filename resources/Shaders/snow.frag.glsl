/*
 * GLSL Fragment Shader code for OpenGL version 3.3
 */

#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D _texture;
uniform vec4 ParticleColor;

void main()
{
    color = texture(_texture, TexCoords) * ParticleColor;
    if (color.r + color.g + color.b >= 2.7f) discard;
}