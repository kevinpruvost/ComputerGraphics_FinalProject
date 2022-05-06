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

const double Pi = acos(-1.0f);

/**
 * @brief Contains information about Tri Faces.
*/
struct Face
{
	std::vector<int> v;
	std::vector<int> vt;
	std::vector<int> vn;

	/** 
	 * @brief Array constructor
	 * @param fd
	*/
	Face(const std::vector<int> & _v, const std::vector<int> & _vt = {}, const std::vector<int> & _vn = {})
		: v{ _v }
		, vt{ _vt }
		, vn{ _vn }
	{
	}
};

/**
 * @brief Contains information about vertices.
*/
struct Vertex
{
	GLfloat x, y, z;
	Vertex(GLfloat x_ = 0.0f, GLfloat y_ = 0.0f, GLfloat z_ = 0.0f) : x(x_), y(y_), z(z_) {}
};

/**
 * @brief Contains information about vertices.
*/
struct VertexNormalTexture
{
	VertexNormalTexture(GLfloat x_ = 0.0f, GLfloat y_ = 0.0f, GLfloat z_ = 0.0f,
		GLfloat nx_ = 0.0f, GLfloat ny_ = 0.0f, GLfloat nz_ = 0.0f,
		GLfloat s_ = 0.0f, GLfloat t_ = 0.0f)
		: xyz(x_, y_, z_)
		, normals(nx_, ny_, nz_)
		, textureCoords(s_, t_)
	{}
	union
	{
		struct { GLfloat x, y, z; };
		glm::fvec3 xyz;
	};
	union
	{
		struct { GLfloat nx, ny, nz; };
		glm::fvec3 normals;
	};
	union
	{
		struct { GLfloat s, t; };
		glm::fvec2 textureCoords;
	};
};