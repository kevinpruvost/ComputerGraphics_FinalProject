/*****************************************************************//**
 * \file   Pbr_Material.cpp
 * \brief  PBR Material source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 19 2022
 *********************************************************************/
#include "Pbr_Material.hpp"

#include "OGL_Implementation\Cubemap\Brdf_Cubemap.hpp"
#include "OGL_Implementation\Rendering\LightRendering.hpp"

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
    if (s_cubemap)
    {
        shader.SetUniformInt("irradianceMap", 0);
        shader.SetUniformInt("prefilterMap", 1);
        shader.SetUniformInt("brdfLUT", 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, s_cubemap->irradianceMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, s_cubemap->prefilterMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, s_cubemap->brdfLUTTexture);
    }

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

    const auto shadowMaps = LightRendering::Get().shadowMaps;
    std::vector<int> values(128, 8);
    for (int i = 0; i < PointLight::GetPointLightsCount(); ++i)
    {
        values[i] = 8 + i;
        glActiveTexture(GL_TEXTURE8 + i);
        glBindTexture(GL_TEXTURE_2D, shadowMaps[i]);
    }
    shader.SetUniformInt("shadowMapsPerPointLight", values);
}
