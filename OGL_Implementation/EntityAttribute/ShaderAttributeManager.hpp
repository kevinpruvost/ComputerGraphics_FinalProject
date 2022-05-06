/*****************************************************************//**
 * \file   ShaderAttributeManager.hpp
 * \brief  ShaderAttributeManager class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 05 2022
 *********************************************************************/
#pragma once

// Project includes
#include "ShaderAttribute.hpp"

template<class T>
/// @brief Concept checking if the type is a primitive appopriate for shader attributes
concept ShaderAttributeTypable =
    std::is_same_v<T, float> 
 || std::is_same_v<T, int>
 || std::is_same_v<T, glm::mat4>;

template<class T>
/// @brief Concept checking if the type is a class appropriate for shader attributes
concept ShaderAttributeTypableClass =
    std::is_same_v<T, glm::mat4>;

template<class T>
/// @brief Concept checking if the type is appropriate for shader attributes (both primitives & classes)
concept ShaderAttributeTypableAny = ShaderAttributeTypable<T> || ShaderAttributeTypableClass<T>;

/**
 * @brief Manages shader attributes for Entities
*/
class ShaderAttributeManager
{
public:
    ShaderAttributeManager();
    ~ShaderAttributeManager();

    template<ShaderAttributeTypable T>
    void SetShaderAttribute(const std::string & name, const T val);
    template<ShaderAttributeTypableClass T>
    void SetShaderAttribute(const std::string & name, const T & val);

    template<ShaderAttributeTypableAny T>
    T * GetShaderAttribute(const std::string & name);
    template<ShaderAttributeTypableAny T>
    const T * GetShaderAttribute(const std::string & name) const;

    std::unordered_map<std::string, std::unique_ptr<ShaderAttribute>> shaderAttributes;
};

#include "ShaderAttributeManager.inl"
