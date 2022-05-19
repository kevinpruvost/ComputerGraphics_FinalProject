/*****************************************************************//**
 * \file   Pbr_Material.hpp
 * \brief  PBR Material class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 19 2022
 *********************************************************************/
#pragma once

// Project includes
#include "EntityAttribute.hpp"
#include "OGL_Implementation\Texture\Texture.hpp"

class Pbr_Material : public EntityAttribute
{
public:
    Pbr_Material(
        const char * albedoMap,
        const char * normalMap,
        const char * metallicMap,
        const char * roughnessMap,
        const char * aoMap);
    ~Pbr_Material();

    void Render(Shader & shader) override;

    Texture albedo, normal, metallic, roughness, ao;
};
