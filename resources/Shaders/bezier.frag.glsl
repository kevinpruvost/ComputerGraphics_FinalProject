#version 460 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D _texture;

void main()
{
    color = texture(_texture, TexCoord);
}