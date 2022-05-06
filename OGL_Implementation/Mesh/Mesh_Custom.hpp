/*****************************************************************//**
 * \file   Mesh_Custom.hpp
 * \brief  Mesh_Custom class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 01 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Mesh_Base.hpp"
#include "Mesh_Geometry.hpp"

/**
 * @brief Mesh built from a given set of vertices
*/
class Mesh_Custom : public Mesh_Base
{
public:
    Mesh_Custom(const std::vector<VertexNormalTexture> & vertices);
    Mesh_Custom(const std::vector<VertexPos> & vertices, const std::vector<VertexNormal> & normals, const std::vector<VertexTextureCoordinates> & textureCoords, const std::vector<Face> & faces);
    Mesh_Custom(const std::vector<VertexPos> & vertices, const std::vector<VertexNormal> & normals, const std::vector<Face> & faces);
    Mesh_Custom(const std::vector<VertexPos> & vertices, const std::vector<Face> & faces);
    ~Mesh_Custom();

    GLuint GetFacesEBO() const;
    bool IsUsingEBO() const;
    Mesh_Base::DrawMode GetDrawMode() const;

    const std::vector<VertexNormalTexture> & GetVertices() const;
    void ModifyVertex(const unsigned int index, const VertexNormalTexture & newVertex);
    void ModifyVertices(const std::vector<VertexNormalTexture> & vertices);

private:
    std::vector<VertexNormalTexture> GenerateAssembledVertices(bool isNormal, bool isTexture) const;
    void ReassignVertex();

private:
    union
    {
        std::vector<VertexNormalTexture> __vertices;
        struct
        {
            std::vector<VertexPos> __v;
            std::vector<VertexNormal> __vN;
            std::vector<VertexTextureCoordinates> __vT;
            std::vector<Face> __faces;
        };
    };
};
