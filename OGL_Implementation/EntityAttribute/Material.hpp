/*****************************************************************//**
 * \file   Material.hpp
 * \brief  Material class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 04 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Texture\Texture.hpp"
#include "EntityAttribute.hpp"

/**
 * @brief Manages Diffuse, Specular mapping and shininess
*/
class Material : public EntityAttribute
{
public:
    Material(const glm::vec3 & diffuse_  = glm::vec3{ 1.0f },
        const glm::vec3 & specular_ = glm::vec3{ 1.0f },
        const float shininess_ = 128.0f
    );
    ~Material();

    void Render(Shader & shader) override;

public:
    enum class MaterialMapType : unsigned char
    {
        Color = 1,
        Texture = 2
    };

    MaterialMapType diffuseMapType, specularMapType;
    union { glm::vec3 diffuseColor;  Texture diffuseTexture; };
    union { glm::vec3 specularColor; Texture specularTexture; };
    float shininess;
};
