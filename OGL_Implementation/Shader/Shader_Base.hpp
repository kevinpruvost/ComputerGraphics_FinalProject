/*****************************************************************//**
 * \file   Shader_Base.hpp
 * \brief  Shader_Base class (from TA's sample)
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Mesh\Mesh.hpp"

// GLAD includes
#include <glad/glad.h>

// C++ includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

/**
 * @brief Contains the real data of the shader to be
 * stored in a static database.
*/
class Shader_Base
{
public:
	/**
	 * @brief Constructs and generates the shader
	 * @param vertexPath 
	 * @param fragmentPath 
	*/
	Shader_Base(const GLchar * vertexPath, const GLchar * fragmentPath);
	/**
	 * @brief Constructs and generates the shader with Vertex Shader, Fragment Shader, TCS Shader & TES Shader
	 * @param vertexPath 
	 * @param fragmentPath 
	 * @param tcsPath 
	 * @param tesPath 
	*/
	Shader_Base(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * tcsPath, const GLchar * tesPath);
	~Shader_Base();

	/**
	 * @brief Uses the current shader
	*/
	void Use() const;

	/**
	 * @brief Returns Shader Program
	 * @return program
	*/
	GLuint Program() const;

	/**
	 * @brief Adds a Global Uniform Buffer Object
	 * @param bindingPoint (location)
	 * @param bindingPointName (name)
	*/
	void AddGlobalUbo(const GLuint bindingPoint, const char * bindingPointName) const;

	void SetUniformInt(const GLchar * uniformName, const GLint nb);
	void SetUniformInt(const GLchar * uniformName, const std::vector<GLint> & nb);

	void SetUniformFloat(const GLchar * uniformName, const GLfloat nb);
	void SetUniformFloat(const GLchar * uniformName, const glm::vec2 & nbs);
	void SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2);
	void SetUniformFloat(const GLchar * uniformName, const glm::vec3 & nbs);
	void SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2, const GLfloat nb3);
	void SetUniformFloat(const GLchar * uniformName, const glm::vec4 & nbs);
	void SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2, const GLfloat nb3, const GLfloat nb4);

	void SetUniformMatrix4f(const GLchar * uniformName, const glm::mat4 & mat);

	GLuint GetUniformId(const GLchar * uniformName);
	GLenum GetPrimitiveMode() const;

private:
	bool CompileShader(const GLchar * shaderPath, const GLenum shaderType, GLuint * shaderID);

private:
	/**
	 * @brief Primitive Mode (if drawing has to be made in some other ways e.g. GL_PATCHES, GL_TRIANGLES...)
	*/
	const GLenum  __primitiveMode;
	/**
	 * @brief Shader program id
	*/
	GLuint __program;
	/**
	 * @brief Contains uniform ids
	*/
	std::unordered_map<std::string, GLuint> __uniformIds;
};