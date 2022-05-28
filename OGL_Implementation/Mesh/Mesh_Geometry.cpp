/*****************************************************************//**
 * \file   Mesh_Geometry.cpp
 * \brief  Mesh Geometry source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 06 2022
 *********************************************************************/
#include "Mesh_Geometry.hpp"

// C++ includes
#include <unordered_map>
#include <utility>

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 
///  HALF EDGE
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

glm::vec4 GetPlaneEquationFromTriangle(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3)
{
	glm::vec3 v1 = p2 - p1, v2 = p3 - p1;
	glm::vec3 plane = { glm::normalize(glm::cross(v1, v2))};
	return { plane, -glm::dot(p1, plane)};
	return { plane, 1.0f };
}

std::vector<std::unique_ptr<HalfEdge>> GenerateHalfEdgesFromVertices(const std::vector<Face> & faces)
{
	std::vector<std::unique_ptr<HalfEdge>> halfEdges;
	halfEdges.reserve(faces.size() * 3);

	std::unordered_map<int, std::unordered_map<int, HalfEdge *>> twinsToAttach;
	for (int i = 0; i < faces.size(); ++i)
	{
		std::array<HalfEdge *, 3> lasts{ nullptr };
		for (int j = 0; j < 3; ++j)
		{
			lasts[j] = halfEdges.emplace_back(new HalfEdge(
				nullptr,// Twin
				nullptr,// Next
				nullptr,// Previous
				i,// Face
				faces[i].v[j]// Origin
			)).get();
		}
		lasts[0]->next = lasts[2]->previous = lasts[1];
		lasts[1]->next = lasts[0]->previous = lasts[2];
		lasts[2]->next = lasts[1]->previous = lasts[0];
		for (int k = 0; k < 3; ++k)
		{
			HalfEdge * currentLast = lasts[k];
			std::unordered_map<int, std::unordered_map<int, HalfEdge *>>::iterator ite = twinsToAttach.find(currentLast->origin);
			std::unordered_map<int, HalfEdge *>::iterator twinLast;
			if (ite != twinsToAttach.end() && (twinLast = ite->second.find(currentLast->next->origin)) != ite->second.end())
			{
				currentLast->twin = twinLast->second;
				twinLast->second->twin = currentLast;
				// Faster
				//twinsToAttach.erase(currentLast->origin);
			}
			else
			{
				if ((ite = twinsToAttach.find(currentLast->next->origin)) == twinsToAttach.end())
					twinsToAttach.emplace(currentLast->next->origin, std::unordered_map<int, HalfEdge *>{ {currentLast->origin, currentLast} });
				else
					ite->second.emplace(currentLast->origin, currentLast);
			}
		}
	}

	/*for (int i = 0; i < halfEdges.size(); ++i)
	{
		if (halfEdges[i]->twin) continue;
		for (int j = 0; j < halfEdges.size(); ++j)
		{
			if (i == j || halfEdges[i]->face == halfEdges[j]->face) continue;

			if (halfEdges[i]->origin == halfEdges[j]->next->origin && halfEdges[i]->next->origin == halfEdges[j]->origin)
			{
				halfEdges[i]->twin = halfEdges[j].get();
				halfEdges[j]->twin = halfEdges[i].get();
				break;
			}
		}
	}*/
	return halfEdges;
}

HalfEdge::HalfEdge(HalfEdge * twin_, HalfEdge * next_, HalfEdge * prev_, int face_, int origin_)
	: twin{twin_}
	, next{next_}
	, previous{prev_}
	, face{face_}
	, origin{origin_}
	, pass{ false }
{
}
