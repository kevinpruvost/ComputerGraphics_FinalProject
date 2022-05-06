/*****************************************************************//**
 * \file   Mesh_Base.cpp
 * \brief  Mesh_Base Source Code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#include "Mesh_Base.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"

Mesh_Base::Mesh_Base()
	: __hasTextureCoordinates{ true }
	, __hasNormals{ true }
{
	LOG_PRINT(Log::LogMainFileName, "Constructed\n");

	glGenVertexArrays(2, &__verticesVAO);
	glGenBuffers(2, &__verticesVBO);
}

Mesh_Base::~Mesh_Base()
{
	LOG_PRINT(Log::LogMainFileName, "Destroyed\n");

	glDeleteVertexArrays(2, &__verticesVAO);
	glDeleteBuffers(2, &__verticesVBO);
}

GLuint Mesh_Base::GetVerticesVAO() const
{
	return __verticesVAO;
}

GLuint Mesh_Base::GetFacesVAO() const
{
	return __facesVAO;
}

GLuint Mesh_Base::GetVerticesVBO() const
{
	return __verticesVBO;
}

GLuint Mesh_Base::GetFacesVBO() const
{
	return __facesVBO;
}

GLuint Mesh_Base::GetVerticesCount() const
{
	return __verticesNVert;
}

GLuint Mesh_Base::GetFacesVerticesCount() const
{
	return __facesNVert;
}

bool Mesh_Base::HasTextureCoordinates() const
{
	return __hasTextureCoordinates;
}

bool Mesh_Base::HasNormals() const
{
	return __hasNormals;
}
