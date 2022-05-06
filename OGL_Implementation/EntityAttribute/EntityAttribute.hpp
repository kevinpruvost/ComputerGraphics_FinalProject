/*****************************************************************//**
 * \file   EntityAttribute.hpp
 * \brief  EntityAttribute class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 05 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Shader\Shader.hpp"

/**
 * @brief Base of all entity attributes like Texture, Material, Mesh, ...
*/
class EntityAttribute
{
public:
    EntityAttribute();
    ~EntityAttribute();

    /**
     * @brief Pure virtual function supposed to contain every manipulation needed
     * for this attribute to be considered in the rendering process, usually during Shader
     * manipulation.
    */
    virtual void Render(Shader & shader) = 0;
};
