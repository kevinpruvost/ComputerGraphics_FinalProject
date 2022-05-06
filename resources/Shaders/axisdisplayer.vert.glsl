/*
 * GLSL Vertex Shader code for OpenGL version 3.3
 */

#version 330 core

// input vertex attributes
layout (location = 0) in vec3 vertex;

uniform mat4 model;
layout (std140) uniform Projection
{
	mat4 projection;
};

out vec3 ourColor;

void main()
{
	gl_Position = projection * model * vec4(vertex.xyz, 1.0);
	ourColor = vertex;
}
