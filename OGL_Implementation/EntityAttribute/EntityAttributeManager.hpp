/*****************************************************************//**
 * \file   EntityAttributeManager.hpp
 * \brief  EntityAttributeManager class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 05 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Material.hpp"
#include "Texture.hpp"

/**
 * @brief Part of an entity that manages its attributes
*/
class EntityAttributeManager
{
public:
    EntityAttributeManager();
    ~EntityAttributeManager();

    /**
     * @brief Adds Default Material
     * @return material
    */
    Material & AddMaterial();
    Texture & AddTexture(const Texture & texture);

    /**
     * @brief Returns material
     * @return material
    */
    Material * GetMaterial();
    Texture * GetTexture();

public:
    enum class EntityAttributeId
    {
        EA_Material = 0,
        EA_Texture  = 1
    };

    std::unordered_map<EntityAttributeId, std::unique_ptr<EntityAttribute>> attributes;
};
