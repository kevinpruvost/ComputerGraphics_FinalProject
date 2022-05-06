/*****************************************************************//**
 * \file   ShaderAttribute.hpp
 * \brief  ShaderAttribute class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 05 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Shader\Shader.hpp"

// GLM includes
#include <glm\glm.hpp>

// C++ includes
#include <string>

/**
 * @brief Shader Attribute, can be assigned any possible type used in GLSL
*/
class ShaderAttribute
{
public:
    ShaderAttribute(const int val);
    ShaderAttribute(const float val);
    ShaderAttribute(const glm::mat4 & val);
    ~ShaderAttribute();

    void Render(const std::string & name, Shader & shader);
    void * GetData();
    const void * GetData() const;

private:
    enum class ShaderAttributeType : unsigned char
    {
        Float  = 0,
        Int    = 1,
        Matrix = 2
    };
    ShaderAttributeType __dataType;
    void * __data;
};
