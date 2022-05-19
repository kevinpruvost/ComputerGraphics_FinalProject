/*****************************************************************//**
 * \file   Pbr_Material.cpp
 * \brief  PBR Material source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 19 2022
 *********************************************************************/
#include "Pbr_Material.hpp"

Pbr_Material::Pbr_Material(const char * albedoMap, const char * normalMap, const char * metallicMap, const char * roughnessMap, const char * aoMap)
{
    if (!GenerateTexture(albedoMap, albedo) || !GenerateTexture(normalMap, normal) || !GenerateTexture(metallicMap, metallic) || !GenerateTexture(roughnessMap, roughness) || !GenerateTexture(aoMap, ao))
    {
        throw std::runtime_error("Couldn't generate PBR Material.");
    }
}

Pbr_Material::~Pbr_Material()
{
}

void Pbr_Material::Render(Shader & shader)
{
    shader.SetUniformInt("albedoMap", 3);
    shader.SetUniformInt("normalMap", 4);
    shader.SetUniformInt("metallicMap", 5);
    shader.SetUniformInt("roughnessMap", 6);
    shader.SetUniformInt("aoMap", 7);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, albedo.GetTexture());
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, normal.GetTexture());
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, metallic.GetTexture());
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, roughness.GetTexture());
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, ao.GetTexture());
}
