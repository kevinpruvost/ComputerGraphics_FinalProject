/*
 * GLSL Fragment Shader code for OpenGL version 3.3
 */

#version 330 core

uniform sampler2D _texture;

in vec2 TexCoords;
out vec4 color;

void main()
{
    color = texture(_texture, TexCoords);
}