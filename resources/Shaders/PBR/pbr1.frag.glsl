/*
 * GLSL Fragment Shader code for OpenGL version 3.3
 */

#version 330 core
#extension GL_ARB_shading_language_include : require

#ifndef SMAA_GLSL_3
#define SSSS_GLSL_3 1
#endif

//#include "resources/Shaders/PBR/SeparableSSS.h"

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
flat in vec3 flatNormal;  

struct Material
{
    bool diffuseUseColor;
    bool specularUseColor;
    vec3 diffuseColor;
    vec3 specularColor;
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    float shininess;
};

struct PointLight // 64 bytes
{
	vec3 position;
    float constant;
    
    vec3 ambient;
    float linear;

    vec3 diffuse;
    float quadratic; 

    vec3 specular;
    float pad0; // Padding
};

struct DirectionLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 128

layout (std140) uniform Lights
{
	PointLight pointLights[NR_POINT_LIGHTS];
//    DirectionLight directionLight;
//    SpotLight spotLight;
    int pointLightsCount;
};

layout (std140) uniform CameraProps
{
    vec4 viewPos;
    mat4 viewProj;
    mat4 view;
	mat4 projection;
};

vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

out vec4 color;
uniform Material material;
uniform bool isNormalFlat = false;
uniform bool useLight = true;

vec3 GetDiffuseMaterial(Material mat)
{
    if (mat.diffuseUseColor)
        return mat.diffuseColor;
    else
        return vec3(texture(mat.diffuseTexture, TexCoords));
}

vec3 GetSpecularMaterial(Material mat)
{
    if (mat.specularUseColor)
        return mat.specularColor;
    else
        return vec3(texture(mat.specularTexture, TexCoords));
}

void main()
{
    if (useLight)
    {
        // Props
        vec3 norm = normalize(isNormalFlat ? flatNormal : Normal);
        vec3 viewDir = normalize(viewPos.xyz - FragPos);
        
        vec3 result = vec3(0.0);
        for(int i = 0; i < pointLightsCount; i++)
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
        color = vec4(result, 1.0);
    }
    else
    {
        color = vec4(GetDiffuseMaterial(material), 1.0);
    }
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * GetDiffuseMaterial(material);
    vec3 diffuse = light.diffuse * diff * GetDiffuseMaterial(material);
    vec3 specular = light.specular * spec * GetSpecularMaterial(material);
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * GetDiffuseMaterial(material);
    vec3 diffuse = light.diffuse * diff * GetDiffuseMaterial(material);
    vec3 specular = light.specular * spec * GetSpecularMaterial(material);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}