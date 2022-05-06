/*
 * GLSL Vertex Shader code for OpenGL version 3.3
 */

#version 330 core

// input vertex attributes
layout (location = 0) in vec4 vertex;

layout (std140) uniform Projection
{
	mat4 projection;
};

out vec2 TexCoords;

void main()
{
	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vec2(vertex.z, 1.0 - vertex.w);
}
