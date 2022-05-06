/*****************************************************************//**
 * \file   Mesh.cpp
 * \brief  Mesh class source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 27 2022
 *********************************************************************/
#include "Mesh.hpp"

static std::vector<std::unique_ptr<Mesh_Base>> meshesDB;

Mesh::Mesh(const GLuint meshId)
	: __meshId(meshId)
{
}

Mesh & Mesh::operator=(const Mesh & mesh)
{
	__meshId = mesh.meshId();
	return *this;
}

GLuint Mesh::meshId() const { return __meshId; }
GLuint Mesh::verticesVAO() const { return meshesDB[__meshId]->GetVerticesVAO(); }
GLuint Mesh::facesVAO() const { return meshesDB[__meshId]->GetFacesVAO(); }
GLuint Mesh::verticesVBO() const { return meshesDB[__meshId]->GetVerticesVBO(); }
GLuint Mesh::facesVBO() const { return meshesDB[__meshId]->GetFacesVBO(); }
GLuint Mesh::verticesNVert() const { return meshesDB[__meshId]->GetVerticesCount(); }
GLuint Mesh::facesNVert() const { return meshesDB[__meshId]->GetFacesVerticesCount(); }

Mesh_Base * Mesh::operator*()
{
	return meshesDB[__meshId].get();
}

const Mesh_Base * Mesh::operator*() const
{
	return meshesDB[__meshId].get();
}

GLuint Mesh::facesEBO() const
{
	return meshesDB[__meshId]->GetFacesEBO();
}

bool Mesh::isUsingEBO() const
{
	return meshesDB[__meshId]->IsUsingEBO();
}

Mesh_Base::DrawMode Mesh::GetDrawMode() const
{
	return meshesDB[__meshId]->GetDrawMode();
}

Mesh GenerateMeshImage()
{
	meshesDB.emplace_back(new Mesh_Image());
	return Mesh(meshesDB.size() - 1);
}

Mesh GenerateMeshSphere(float radius, int sectors, int stacks, bool smooth)
{
	meshesDB.emplace_back(new Mesh_Sphere(radius, sectors, stacks, smooth));
	return Mesh(meshesDB.size() - 1);
}

Mesh GenerateMesh(const std::vector<VertexNormalTexture> & vertices)
{
	meshesDB.emplace_back(new Mesh_Custom(vertices));
	return Mesh(meshesDB.size() - 1);
}

Mesh GenerateMesh(const Obj & obj)
{
	meshesDB.emplace_back(new Mesh_Obj(obj));
	return Mesh(meshesDB.size() - 1);
}

Mesh GenerateMesh(const Mesh & mesh)
{
	return Mesh(mesh);
}

Mesh GenerateMesh(const uint16_t meshId)
{
	return Mesh(meshId);
}
