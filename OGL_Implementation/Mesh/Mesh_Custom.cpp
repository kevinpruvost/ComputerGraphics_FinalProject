/*****************************************************************//**
 * \file   Mesh_Custom.cpp
 * \brief  Mesh_Custom source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 01 2022
 *********************************************************************/
#include "Mesh_Custom.hpp"

// C++ includes
#include <stdexcept>

Mesh_Custom::Mesh_Custom(const std::vector<VertexNormalTexture> & vertices_)
    : Mesh_Base()
{
    __vertices = vertices_;

    LoadFaces(__vertices);

    std::vector<VertexPos> onlyPos;
    onlyPos.reserve(__vertices.size());

    for (auto & vertex : __vertices)
        onlyPos.emplace_back(vertex.x, vertex.y, vertex.z);

    LoadVertices(onlyPos);
}

Mesh_Custom::Mesh_Custom(const std::vector<VertexPos> & vertices, const std::vector<VertexNormal> & normals, const std::vector<VertexTextureCoordinates> & textureCoords, const std::vector<Face> & faces)
    : Mesh_Base(faces, vertices, normals, textureCoords)
{
    LoadVertices(__v);
    const auto vnts = GenerateAssembledVertices(true, true);
    LoadFaces(vnts);
}

Mesh_Custom::Mesh_Custom(const std::vector<VertexPos> & vertices, const std::vector<VertexNormal> & normals, const std::vector<Face> & faces)
    : Mesh_Base(faces, vertices, normals)
{
    LoadVertices(__v);
    const auto vnts = GenerateAssembledVertices(true, false);
    LoadFaces(vnts);
}

Mesh_Custom::Mesh_Custom(const std::vector<VertexPos> & vertices, const std::vector<Face> & faces)
    : Mesh_Base(faces, vertices)
{
    LoadVertices(__v);
    const auto vnts = GenerateAssembledVertices(false, false);
    LoadFaces(vnts);
}

Mesh_Custom::~Mesh_Custom()
{
}

GLuint Mesh_Custom::GetFacesEBO() const
{
    return 0;
}

bool Mesh_Custom::IsUsingEBO() const
{
    return false;
}

Mesh_Base::DrawMode Mesh_Custom::GetDrawMode() const
{
    return DrawMode::DrawArrays;
}

std::vector<VertexNormalTexture> & Mesh_Custom::GetVertices()
{
    return __vertices;
}

const std::vector<VertexNormalTexture> & Mesh_Custom::GetVertices() const
{
    return __vertices;
}

void Mesh_Custom::ModifyVertex(const unsigned int index, const VertexNormalTexture & newVertex)
{
    if (index >= __vertices.size()) throw std::runtime_error("Vertex out of bound");

    __vertices[index] = newVertex;

    ReassignVertex();
}

void Mesh_Custom::ModifyVertices(const std::vector<VertexNormalTexture> & vertices)
{
    __vertices = vertices;

    ReassignVertex();
    
    __facesNVert = __verticesNVert = __vertices.size();
}

const std::vector<VertexPos> * Mesh_Custom::GetVerticesPos() const
{
    return &__v;
}

const std::vector<VertexNormal> * Mesh_Custom::GetVerticesNormals() const
{
    return &__vN;
}

const std::vector<VertexTextureCoordinates> * Mesh_Custom::GetVerticesTextureCoordinates() const
{
    return &__vT;
}

const std::vector<Face> * Mesh_Custom::GetFaces() const
{
    return &__faces;
}

inline void Mesh_Custom::ReassignVertex()
{
    glBindVertexArray(__verticesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, __verticesVAO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, __vertices.size() * sizeof(VertexNormalTexture), __vertices.data());

    glBindVertexArray(__facesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, __facesVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, __vertices.size() * sizeof(VertexNormalTexture), __vertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
