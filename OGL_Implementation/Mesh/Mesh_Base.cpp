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

void Mesh_Base::LoadVertices(const std::vector<VertexPos> & vertices)
{
	glBindVertexArray(__verticesVAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, __verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexPos), vertices.data(), GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	__verticesNVert = vertices.size();
}

void Mesh_Base::LoadFaces(const std::vector<VertexNormalTexture> & vertices)
{
	glBindVertexArray(__facesVAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, __facesVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexNormalTexture), vertices.data(), GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	__facesNVert = vertices.size();
}
