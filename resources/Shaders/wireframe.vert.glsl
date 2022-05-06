/*
 * GLSL Vertex Shader code for OpenGL version 3.3
 */

#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
layout (std140) uniform CameraProps
{
    vec4 viewPos;
    mat4 viewProj;
    mat4 view;
	mat4 projection;
};

void main()
{
	gl_Position = viewProj * model * vec4(aPos, 1.0f);
}
