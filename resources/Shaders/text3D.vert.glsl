/*
 * GLSL Vertex Shader code for OpenGL version 3.3
 */

#version 330 core

// input vertex attributes
layout (location = 0) in vec4 vertex;

layout (std140) uniform CameraProps
{
    vec4 viewPos;
    mat4 viewProj;
    mat4 view;
	mat4 projection;
};
uniform mat4 model;

out vec2 TexCoords;

// See https://geeks3d.developpez.com/billboarding-vertex-shader/ for billboarding

void main()
{
	mat4 modelView = view * model;

	// 1st column
	modelView[0][0] = 1.0; 
	modelView[0][1] = 0.0; 
	modelView[0][2] = 0.0;

    // 2nd column
    modelView[1][0] = 0.0;
    modelView[1][1] = 1.0;
    modelView[1][2] = 0.0;

	// 3rd column 
	modelView[2][0] = 0.0;
	modelView[2][1] = 0.0;
	modelView[2][2] = 1.0;
	gl_Position = projection * modelView * vec4(vertex.xy, 0.0f, 1.0f);
	TexCoords = vec2(vertex.z, 1.0f - vertex.w);
}
