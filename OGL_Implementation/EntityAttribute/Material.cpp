/*****************************************************************//**
 * \file   Material.cpp
 * \brief  Material Source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 04 2022
 *********************************************************************/
#include "Material.hpp"

#include "OGL_Implementation\Rendering\LightRendering.hpp"

Material::Material(const glm::vec3 & diffuse_, const glm::vec3 & specular_, const float shininess_)
    : EntityAttribute()
    , diffuseColor{ diffuse_ }
    , specularColor{ specular_ }
    , shininess{ shininess_ }
    , diffuseMapType{ MaterialMapType::Color }
    , specularMapType{ MaterialMapType::Color }
{
    
}

Material::~Material()
{
}

void Material::Render(Shader & shader)
{
    int textureID = 0;
    auto checker = [&](const MaterialMapType mapType, const glm::vec3 & color, const Texture texture,
                       const char * useColorName, const char * colorName, const char * textureName) {
        switch (mapType)
        {
            case MaterialMapType::Color:
                shader.SetUniformInt(useColorName, 1);
                shader.SetUniformFloat(colorName, color);
                break;
            case MaterialMapType::Texture:
                shader.SetUniformInt(useColorName, 0);
                shader.SetUniformInt(textureName, textureID);
                glActiveTexture(GL_TEXTURE0 + textureID++);
                glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
                break;
        }
    };
    checker(diffuseMapType, diffuseColor, diffuseTexture, "material.diffuseUseColor", "material.diffuseColor", "material.diffuseTexture");
    checker(specularMapType, specularColor, specularTexture, "material.specularUseColor", "material.specularColor", "material.specularTexture");

    const auto shadowMaps = LightRendering::Get().shadowMaps;
    std::vector<int> values(128, textureID);
    for (int i = 0; i < PointLight::GetPointLightsCount(); ++i)
    {
        values[i] = textureID + i;
        glActiveTexture(GL_TEXTURE0 + values[i]);
        glBindTexture(GL_TEXTURE_2D, shadowMaps[i]);
    }
    shader.SetUniformInt("shadowMapsPerPointLight", values);

    shader.SetUniformFloat("material.shininess", shininess);
}
