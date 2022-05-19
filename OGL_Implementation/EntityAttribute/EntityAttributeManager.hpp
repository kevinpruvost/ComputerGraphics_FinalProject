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
#include "Pbr_Material.hpp"

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
    /**
     * @brief Adds Default Material
     * @return material
    */
    Pbr_Material & AddPbrMaterial(const char * albedoMap,
        const char * normalMap,
        const char * metallicMap,
        const char * roughnessMap,
        const char * aoMap);

    /**
     * @brief Returns material
     * @return material
    */
    Material * GetMaterial();
    /**
     * @brief Returns material
     * @return material
    */
    Pbr_Material * GetPbrMaterial();

public:
    enum class EntityAttributeId
    {
        EA_Material     = 0,
        EA_PbrMaterial  = 1
    };

    std::unordered_map<EntityAttributeId, std::unique_ptr<EntityAttribute>> attributes;
};
