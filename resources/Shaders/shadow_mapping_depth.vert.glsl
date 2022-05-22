#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    vec4 pos = lightSpaceMatrix * model * vec4(aPos, 1.0);
    gl_Position = pos;
}