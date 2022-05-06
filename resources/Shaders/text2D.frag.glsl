/*
 * GLSL Fragment Shader code for OpenGL version 3.3
 */

#version 330 core

uniform sampler2D text;
uniform vec3 textColor;

in vec2 TexCoords;
out vec4 color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}