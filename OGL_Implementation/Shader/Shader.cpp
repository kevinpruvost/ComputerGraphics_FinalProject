/*****************************************************************//**
 * \file   Shader.cpp
 * \brief  Shader source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 04 2022
 *********************************************************************/
#include "Shader.hpp"

static std::vector<std::unique_ptr<Shader_Base>> shaderDB;

Shader::Shader(const GLuint shaderId)
    : __shaderId{ shaderId }
{
}

GLuint Shader::Program() const
{
    return shaderDB[__shaderId]->Program();
}

void Shader::Use() const
{
    shaderDB[__shaderId]->Use();
}

void Shader::AddGlobalUbo(const GLuint bindingPoint, const char * bindingPointName) const
{
    shaderDB[__shaderId]->AddGlobalUbo(bindingPoint, bindingPointName);
}

GLuint Shader::GetShaderDatabaseID() const
{
    return __shaderId;
}

void Shader::SetUniformInt(const GLchar * uniformName, const GLint nb)
{
    shaderDB[__shaderId]->SetUniformInt(uniformName, nb);
}

void Shader::SetUniformInt(const GLchar * uniformName, const std::vector<GLint> & nb)
{
    shaderDB[__shaderId]->SetUniformInt(uniformName, nb);
}

Shader GenerateShader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
    shaderDB.emplace_back(new Shader_Base(vertexPath, fragmentPath));
    return Shader(shaderDB.size() - 1);
}

Shader GenerateShader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * geometryPath)
{
    shaderDB.emplace_back(new Shader_Base(vertexPath, fragmentPath, geometryPath));
    return Shader(shaderDB.size() - 1);
}

Shader GenerateShader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * tcsPath, const GLchar * tesPath)
{
    shaderDB.emplace_back(new Shader_Base(vertexPath, fragmentPath, tcsPath, tesPath));
    return Shader(shaderDB.size() - 1);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const GLfloat nb)
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nb);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const glm::vec2 & nbs)
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nbs);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2)
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nb1, nb2);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const glm::vec3 & nbs)
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nbs);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2, const GLfloat nb3)
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nb1, nb2, nb3);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const glm::vec4 & nbs)
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nbs);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2, const GLfloat nb3, const GLfloat nb4)
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nb1, nb2, nb3, nb4);
}

void Shader::SetUniformMatrix4f(const GLchar * uniformName, const glm::mat4 & mat)
{
    shaderDB[__shaderId]->SetUniformMatrix4f(uniformName, mat);
}

Shader_Base * Shader::operator*()
{
    return shaderDB[__shaderId].get();
}

const Shader_Base * Shader::operator*() const
{
    return shaderDB[__shaderId].get();
}
