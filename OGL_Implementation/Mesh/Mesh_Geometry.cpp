/*****************************************************************//**
 * \file   Mesh_Geometry.cpp
 * \brief  Mesh Geometry source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 06 2022
 *********************************************************************/
#include "Mesh_Geometry.hpp"

Face::Face(const std::vector<int> & _v, const std::vector<int> & _vt, const std::vector<int> & _vn)
	: v{ _v }
	, vt{ _vt }
	, vn{ _vn }
{
}

VertexNormalTexture::VertexNormalTexture(GLfloat x_, GLfloat y_, GLfloat z_, GLfloat nx_, GLfloat ny_, GLfloat nz_, GLfloat s_, GLfloat t_)
	: xyz(x_, y_, z_)
	, normals(nx_, ny_, nz_)
	, textureCoords(s_, t_)
{
}