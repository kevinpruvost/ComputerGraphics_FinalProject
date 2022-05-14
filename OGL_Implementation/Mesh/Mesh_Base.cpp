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

// C++ includes
#include <stdexcept>
#include <functional>

Mesh_Base::Mesh_Base()
	: __hasTextureCoordinates{ true }
	, __hasNormals{ true }
{
	LOG_PRINT(Log::LogMainFileName, "Constructed\n");

	glGenVertexArrays(2, &__verticesVAO);
	glGenBuffers(2, &__verticesVBO);
}

Mesh_Base::Mesh_Base(const std::vector<Face> & faces, const std::vector<VertexPos> & v, const std::vector<VertexNormal> & vN, const std::vector<VertexTextureCoordinates> & vT)
	: __hasTextureCoordinates{ !vT.empty() }
	, __hasNormals{ !vN.empty() }
	, __faces{ faces }
	, __v{ v }
	, __vN{ vN }
	, __vT{ vT }
{
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

const std::vector<VertexPos> * Mesh_Base::GetVerticesPos() const
{
	return nullptr;
}

const std::vector<VertexNormal> * Mesh_Base::GetVerticesNormals() const
{
	return nullptr;
}

const std::vector<VertexTextureCoordinates> * Mesh_Base::GetVerticesTextureCoordinates() const
{
	return nullptr;
}

const std::vector<Face> * Mesh_Base::GetFaces() const
{
	return nullptr;
}

bool Mesh_Base::HasTextureCoordinates() const
{
	return __hasTextureCoordinates;
}

bool Mesh_Base::HasNormals() const
{
	return __hasNormals;
}

void Mesh_Base::GenerateNormals(bool smooth)
{
	__vN.clear();
	__vN.resize(__v.size(), glm::vec3(0.0f));
	__verticesNVert = __v.size();

	for (auto & face : __faces)
	{
		face.vn = face.v;
		const VertexPos & p0 = __v[face.v[0]], & p1 = __v[face.v[1]], & p2 = __v[face.v[2]];
		glm::vec3 faceNormal = glm::cross(p0 - p1, p1 - p2);

		if (smooth)
		{
			__vN[face.vn[0]] += faceNormal;
			__vN[face.vn[1]] += faceNormal;
			__vN[face.vn[2]] += faceNormal;
		}
		else
		{
			__vN[face.vn[0]] = faceNormal;
			__vN[face.vn[1]] = faceNormal;
			__vN[face.vn[2]] = faceNormal;
		}
	}
	LoadFaces(GenerateAssembledVertices(true, false));
}

void Mesh_Base::SetGeometry(const std::vector<Face> & faces, const std::vector<VertexPos> & v, const std::vector<VertexNormal> & vN, const std::vector<VertexTextureCoordinates> & vT)
{
	__hasTextureCoordinates = !vT.empty();
	__hasNormals = !vN.empty();
	__faces = faces;
	__v = v;
	__vN = vN;
	__vT = vT;
}

void Mesh_Base::UpdateVerticesToApi()
{
	glBindVertexArray(__verticesVAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, __verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, __v.size() * sizeof(VertexPos), __v.data(), GL_DYNAMIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	__verticesNVert = __v.size();
}

void Mesh_Base::LoadVertices(const std::vector<VertexPos> & vertices)
{
	glBindVertexArray(__verticesVAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, __verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexPos), vertices.data(), GL_DYNAMIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexNormalTexture), vertices.data(), GL_DYNAMIC_DRAW);
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

std::vector<VertexNormalTexture> Mesh_Base::GenerateAssembledVertices(bool isNormal, bool isTexture) const
{
	std::vector<VertexNormalTexture> res;
	res.reserve(__faces.size() * 3);

	if (__vN.empty()) isNormal = false;
	if (__vT.empty()) isTexture = false;

	const std::array<std::function<void(int, int)>, 4> lambdas = {
		[&](int i, int j) {
			const int vid = __faces[i].v[j];
			const int vnid = __faces[i].vn[j];
			const int vtid = __faces[i].vt[j];
			res.emplace_back(__v[vid].x, __v[vid].y, __v[vid].z, __vN[vnid].x, __vN[vnid].y, __vN[vnid].z, __vT[vtid].x, __vT[vtid].y);
		},
		[&](int i, int j) {
			const int vid = __faces[i].v[j];
			const int vnid = __faces[i].vn[j];
			res.emplace_back(__v[vid].x, __v[vid].y, __v[vid].z, __vN[vnid].x, __vN[vnid].y, __vN[vnid].z);
		},
		[&](int i, int j) {
			const int vid = __faces[i].v[j];
			const int vtid = __faces[i].vt[j];
			VertexNormalTexture vnt;
			vnt.xyz = { __v[vid].x, __v[vid].y, __v[vid].z };
			vnt.s = __vT[vtid].x;
			vnt.t = __vT[vtid].y;
			res.push_back(vnt);
		},
		[&](int i, int j) {
			const int vid = __faces[i].v[j];
			res.emplace_back(__v[vid].x, __v[vid].y, __v[vid].z);
		}
	};

	const std::function<void(int, int)> * l;
	if (isNormal && isTexture) l = &lambdas[0];
	else if (isNormal) l = &lambdas[1];
	else if (isTexture) l = &lambdas[2];
	else l = &lambdas[3];

	for (int i = 0; i < __faces.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			(*l)(i, j);
		}
	}
	return res;
}