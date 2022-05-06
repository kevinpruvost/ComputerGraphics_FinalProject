/*
 * GLSL Fragment Shader code for OpenGL version 3.3
 */

#version 330 core

// each face is of one color rather than interpolation
uniform vec3 ourColor;

out vec4 color;

void main()
{
	color = vec4(ourColor, 1.0f);
}
