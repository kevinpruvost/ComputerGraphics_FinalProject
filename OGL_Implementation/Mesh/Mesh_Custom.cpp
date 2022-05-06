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
#include <functional>

Mesh_Custom::Mesh_Custom(const std::vector<VertexNormalTexture> & vertices_)
    : __vertices{ vertices_ }
{
    LoadFaces(__vertices);

    std::vector<VertexPos> onlyPos;
    onlyPos.reserve(__vertices.size());

    for (auto & vertex : __vertices)
        onlyPos.emplace_back(vertex.x, vertex.y, vertex.z);

    LoadVertices(onlyPos);
}

Mesh_Custom::Mesh_Custom(const std::vector<VertexPos> & vertices, const std::vector<VertexNormal> & normals, const std::vector<VertexTextureCoordinates> & textureCoords, const std::vector<Face> & faces)
    : __v{ vertices }
    , __vN{ normals }
    , __vT{ textureCoords }
    , __faces{ faces }
{
    LoadVertices(__v);
    const auto vnts = GenerateAssembledVertices(true, true);
    LoadFaces(vnts);
}

Mesh_Custom::Mesh_Custom(const std::vector<VertexPos> & vertices, const std::vector<VertexNormal> & normals, const std::vector<Face> & faces)
    : __v{ vertices }
    , __vN{ normals }
    , __faces{ faces }
{
    LoadVertices(__v);
    const auto vnts = GenerateAssembledVertices(true, false);
    LoadFaces(vnts);
}

Mesh_Custom::Mesh_Custom(const std::vector<VertexPos> & vertices, const std::vector<Face> & faces)
    : __v{ vertices }
    , __faces{ faces }
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

std::vector<VertexNormalTexture> Mesh_Custom::GenerateAssembledVertices(bool isNormal, bool isTexture) const
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
