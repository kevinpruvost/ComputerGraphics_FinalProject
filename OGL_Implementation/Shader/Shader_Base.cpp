/*****************************************************************//**
 * \file   Shader_Base.cpp
 * \brief  Source code of Shader class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 27 2022
 *********************************************************************/
#include "Shader_Base.hpp"

// GLM includes
#include <glm\gtc\type_ptr.hpp>

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"
#include "Shadinclude.hpp"

/**
 * @brief Program memory for Use() because it is a heavy process
*/
static GLuint programUsed = -1;

Shader_Base::Shader_Base(const GLchar * vertexPath, const GLchar * fragmentPath)
	: __primitiveMode(GL_TRIANGLES)
{
	GLuint vertexID, fragID;
	if (!CompileShader(vertexPath,   GL_VERTEX_SHADER, &vertexID) ||
		!CompileShader(fragmentPath, GL_FRAGMENT_SHADER, &fragID))
	{
		throw std::runtime_error("Couldn't create shader.");
	}

	__program = glCreateProgram();
	glAttachShader(__program, vertexID);
	glAttachShader(__program, fragID);
	glLinkProgram(__program);

	// Print linking errors if any
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(__program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(__program, 512, NULL, infoLog);
		LOG_PRINT(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED for %s: %s\n", vertexPath, infoLog);
		throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
	}

	glDeleteShader(vertexID);
	glDeleteShader(fragID);
}

Shader_Base::Shader_Base(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * tcsPath, const GLchar * tesPath)
	: __primitiveMode(GL_PATCHES)
{
	GLuint vertexID, fragID, tcsID, tesID;
	if (!CompileShader(vertexPath,   GL_VERTEX_SHADER, &vertexID) ||
		!CompileShader(fragmentPath, GL_FRAGMENT_SHADER, &fragID) ||
		!CompileShader(tcsPath,      GL_TESS_CONTROL_SHADER, &tcsID) ||
		!CompileShader(tesPath,      GL_TESS_EVALUATION_SHADER, &tesID))
	{
		throw std::runtime_error("Couldn't create shader.");
	}

	__program = glCreateProgram();
	glAttachShader(__program, vertexID);
	glAttachShader(__program, fragID);
	glAttachShader(__program, tcsID);
	glAttachShader(__program, tesID);
	glLinkProgram(__program);

	// Print linking errors if any
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(__program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(__program, 512, NULL, infoLog);
		LOG_PRINT(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED: %s\n", infoLog);
		throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
	}

	glDeleteShader(vertexID);
	glDeleteShader(fragID);
	glDeleteShader(tcsID);
	glDeleteShader(tesID);
}

Shader_Base::Shader_Base(const GLchar * vertexPath, const GLchar * fragmentPath, const GLchar * geometryPath)
	: __primitiveMode(GL_TRIANGLES)
{
	GLuint vertexID, fragID, geometryID;
	if (!CompileShader(vertexPath, GL_VERTEX_SHADER, &vertexID) ||
		!CompileShader(fragmentPath, GL_FRAGMENT_SHADER, &fragID) ||
		!CompileShader(geometryPath, GL_GEOMETRY_SHADER, &geometryID))
	{
		throw std::runtime_error("Couldn't create shader.");
	}

	__program = glCreateProgram();
	glAttachShader(__program, vertexID);
	glAttachShader(__program, fragID);
	glAttachShader(__program, geometryID);
	glLinkProgram(__program);

	// Print linking errors if any
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(__program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(__program, 512, NULL, infoLog);
		LOG_PRINT(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED: %s\n", infoLog);
		throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED");
	}

	glDeleteShader(vertexID);
	glDeleteShader(fragID);
	glDeleteShader(geometryID);
}

Shader_Base::~Shader_Base()
{
	if (__program == programUsed) programUsed = (GLuint)(-1);
	glDeleteProgram(__program);
}

bool Shader_Base::CompileShader(const GLchar * shaderPath, const GLenum shaderType, GLuint * shaderID)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string code;
	try
	{
		code = ShadInclude::load(shaderPath);
	} catch (std::ifstream::failure e) {
		LOG_PRINT(stderr, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ for %s: Error code: %d\n", shaderPath, e.code().value());
		return false;
	}
	const GLchar * shaderCode = code.c_str();
	// 2. Compile shaders
	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	*shaderID = glCreateShader(shaderType);
	glShaderSource(*shaderID, 1, &shaderCode, NULL);
	glCompileShader(*shaderID);
	// Print compile errors if any
	glGetShaderiv(*shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*shaderID, 512, NULL, infoLog);
		LOG_PRINT(stderr, "ERROR::SHADER::COMPILATION_FAILED for %s:\n%s\n", shaderPath, infoLog);
		return false;
	}
	return true;
}

GLuint Shader_Base::Program() const
{
	return __program;
}

void Shader_Base::Use() const
{
	if (programUsed != __program)
	{
		programUsed = __program;
		glUseProgram(__program);
	}
}

void Shader_Base::AddGlobalUbo(const GLuint bindingPoint, const char * bindingPointName) const
{
	GLuint id = glGetUniformBlockIndex(__program, bindingPointName);
	glUniformBlockBinding(__program, id, bindingPoint);
}

void Shader_Base::SetUniformInt(const GLchar * uniformName, const GLint nb)
{
	glUniform1i(GetUniformId(uniformName), nb);
}

void Shader_Base::SetUniformInt(const GLchar * uniformName, const std::vector<GLint> & nb)
{
	glUniform1iv(GetUniformId(uniformName), nb.size(), nb.data());
}

void Shader_Base::SetUniformFloat(const GLchar * uniformName, const GLfloat nb)
{
	glUniform1f(GetUniformId(uniformName), nb);
}

void Shader_Base::SetUniformFloat(const GLchar * uniformName, const glm::vec2 & nbs)
{
	glUniform2fv(GetUniformId(uniformName), 1, glm::value_ptr(nbs));
}

void Shader_Base::SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2)
{
	glUniform2f(GetUniformId(uniformName), nb1, nb2);
}

void Shader_Base::SetUniformFloat(const GLchar * uniformName, const glm::vec3 & nbs)
{
	glUniform3fv(GetUniformId(uniformName), 1, glm::value_ptr(nbs));
}

void Shader_Base::SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2, const GLfloat nb3)
{
	glUniform3f(GetUniformId(uniformName), nb1, nb2, nb2);
}

void Shader_Base::SetUniformFloat(const GLchar * uniformName, const glm::vec4 & nbs)
{
	glUniform4fv(GetUniformId(uniformName), 1, glm::value_ptr(nbs));
}

void Shader_Base::SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2, const GLfloat nb3, const GLfloat nb4)
{
	glUniform4f(GetUniformId(uniformName), nb1, nb2, nb2, nb3);
}

void Shader_Base::SetUniformMatrix4f(const GLchar * uniformName, const glm::mat4 & mat)
{
	glUniformMatrix4fv(GetUniformId(uniformName), 1, GL_FALSE, glm::value_ptr(mat));
}

GLuint Shader_Base::GetUniformId(const GLchar * uniformName)
{
	if (__uniformIds.contains(uniformName))
		return __uniformIds.at(uniformName);
	return __uniformIds[uniformName] = glGetUniformLocation(__program, uniformName);
}

GLenum Shader_Base::GetPrimitiveMode() const
{
	return __primitiveMode;
}
