/*****************************************************************//**
 * \file   Mesh_Geometry.hpp
 * \brief  Geometry related functions & classes
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#pragma once

// C++ includes
#include <array>
#include <cmath>
#include <vector>

// GLM includes
#include <glm\glm.hpp>

// GLAD includes
#include <glad\glad.h>

const double Pi = acos(-1.0f);

/**
 * @brief Contains information about Tri Faces.
*/
struct Face
{
	/** 
	 * @brief Array constructor
	 * @param fd
	*/
	Face(const std::vector<int> & _v, const std::vector<int> & _vt = {}, const std::vector<int> & _vn = {});

	std::vector<int> v;
	std::vector<int> vt;
	std::vector<int> vn;
};

typedef glm::vec3 VertexPos;
typedef VertexPos VertexNormal;
typedef glm::vec2 VertexTextureCoordinates;

/**
 * @brief Contains information about vertices.
*/
struct VertexNormalTexture
{
	VertexNormalTexture(GLfloat x_ = 0.0f, GLfloat y_ = 0.0f, GLfloat z_ = 0.0f,
		GLfloat nx_ = 0.0f, GLfloat ny_ = 0.0f, GLfloat nz_ = 0.0f,
		GLfloat s_ = 0.0f, GLfloat t_ = 0.0f);

	union
	{
		struct { GLfloat x, y, z; };
		glm::vec3 xyz;
	};
	union
	{
		struct { GLfloat nx, ny, nz; };
		glm::vec3 normals;
	};
	union
	{
		struct { GLfloat s, t; };
		glm::vec2 textureCoords;
	};
};