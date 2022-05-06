#include "ShaderAttribute.hpp"

ShaderAttribute::ShaderAttribute(const int val)
    : __data{new int(val)}
    , __dataType{ ShaderAttributeType::Int }
{
}

ShaderAttribute::ShaderAttribute(const float val)
    : __data{new float(val)}
    , __dataType{ ShaderAttributeType::Float }
{
}

ShaderAttribute::ShaderAttribute(const glm::mat4 & val)
    : __data{ new glm::mat4(val) }
    , __dataType{ ShaderAttributeType::Matrix }
{
}

ShaderAttribute::~ShaderAttribute()
{
    if (__data) delete __data;
}

void ShaderAttribute::Render(const std::string & name, Shader & shader)
{
    switch (__dataType)
    {
        case ShaderAttributeType::Int:
            shader.SetUniformInt(name.c_str(), *(int *)__data);
            break;
        case ShaderAttributeType::Float:
            shader.SetUniformFloat(name.c_str(), *(float *)__data);
            break;
        case ShaderAttributeType::Matrix:
            shader.SetUniformMatrix4f(name.c_str(), *(glm::mat4 *)__data);
            break;
    }
}

void * ShaderAttribute::GetData()
{
    return __data;
}

const void * ShaderAttribute::GetData() const
{
    return __data;
}
