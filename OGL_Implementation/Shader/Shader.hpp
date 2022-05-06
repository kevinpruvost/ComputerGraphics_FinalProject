/*****************************************************************//**
 * \file   Shader.hpp
 * \brief  Shader class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 04 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Shader_Base.hpp"

// C++ includes
#include <cstdint>
#include <vector>
#include <unordered_map>

/**
 * @brief Contains Shader Id and methods related
 * to the shader database.
 * It also helps having a much lighter class because Shader_Base weighs 4 bytes
 * and Shader only 2 bytes.
 * And most importantly, the usage is way easier, because copies, constructors, destructors,
 * will not affect the original shader
*/
class Shader
{
public:
    /**
     * @brief Constructor from Shader ID.
     * @param meshId
    */
    Shader(const GLuint shaderId);
    ~Shader() = default;

public:
    GLuint Program() const;
    void Use() const;
    void AddGlobalUbo(const GLuint bindingPoint, const char * bindingPointName) const;
    GLuint GetShaderDatabaseID() const;

    void SetUniformInt(const GLchar * uniformName, const GLint nb);

    void SetUniformFloat(const GLchar * uniformName, const GLfloat nb);
    void SetUniformFloat(const GLchar * uniformName, const glm::vec2 & nbs);
    void SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2);
    void SetUniformFloat(const GLchar * uniformName, const glm::vec3 & nbs);
    void SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2, const GLfloat nb3);
    void SetUniformFloat(const GLchar * uniformName, const glm::vec4 & nbs);
    void SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2, const GLfloat nb3, const GLfloat nb4);

    void SetUniformMatrix4f(const GLchar * uniformName, const glm::mat4 & mat);

    Shader_Base * operator*();
    const Shader_Base * operator*() const;

public:
    GLuint __shaderId;
};

/*
 * @brief Generates shader from GLSL files
 * @param Vertex Path
 * @param Fragment Path
*/
Shader GenerateShader(const GLchar * vertexPath, const GLchar * fragmentPath);

/*
 * @brief Generates shader from GLSL files
 * @param Vertex Path
 * @param Fragment Path
 * @param TCS Path
 * @param TES Path
*/
Shader GenerateShader(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * tcsPath, const GLchar * tesPath);
