/*
 * GLSL Vertex Shader code for OpenGL version 3.3
 */

#version 330 core

// input vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
layout (std140) uniform CameraProps
{
    vec4 viewPos;
    mat4 viewProj;
    mat4 view;
	mat4 projection;
};

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
flat out vec3 flatNormal;

void main()
{
	gl_Position = viewProj * model * vec4(aPos, 1.0f);
	TexCoords = aTexCoords;
	FragPos = vec3(model * vec4(aPos, 1.0));
    flatNormal = Normal = mat3(transpose(inverse(model))) * aNormal;
}
