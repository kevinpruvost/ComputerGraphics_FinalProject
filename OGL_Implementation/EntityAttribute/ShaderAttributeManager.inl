/*****************************************************************//**
 * \file   ShaderAttributeManager.inl
 * \brief  ShaderAttributeManager inline code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 05 2022
 *********************************************************************/
#include "ShaderAttributeManager.hpp"

template<ShaderAttributeTypable T>
inline void ShaderAttributeManager::SetShaderAttribute(const std::string & name, const T val)
{
    shaderAttributes[name] = std::make_unique<ShaderAttribute>(val);
}

template<ShaderAttributeTypableClass T>
inline void ShaderAttributeManager::SetShaderAttribute(const std::string & name, const T & val)
{
    shaderAttributes[name] = std::make_unique<ShaderAttribute>(val);
}

template<ShaderAttributeTypableAny T>
inline T * ShaderAttributeManager::GetShaderAttribute(const std::string & name)
{
    if (!shaderAttributes.contains(name))
    {
        return nullptr;
    }
    return (T *)shaderAttributes[name].get()->GetData();
}

template<ShaderAttributeTypableAny T>
inline const T * ShaderAttributeManager::GetShaderAttribute(const std::string & name) const
{
    if (!shaderAttributes.contains(name))
    {
        return nullptr;
    }
    return (const T *)shaderAttributes[name].get()->GetData();
}
