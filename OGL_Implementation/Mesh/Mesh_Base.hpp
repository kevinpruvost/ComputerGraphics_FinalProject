/*****************************************************************//**
 * \file   Mesh_Base.hpp
 * \brief  Mesh_Base class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Mesh_Geometry.hpp"

// GLAD includes
#include <GLAD\glad.h>

// C++ includes
#include <type_traits>
#include <vector>
#include <string>

class Mesh_Base;

/// @brief Concept checking if the type is a Mesh_Base child.
template<class T>
concept Mesh_Based = std::is_base_of_v<Mesh_Base, T>;

/**
 * @brief Contains the real data of the mesh to be
 * stored in a static database.
*/
class Mesh_Base
{
protected:
    Mesh_Base();
    Mesh_Base(const std::vector<Face> & faces, const std::vector<VertexPos> & v, const std::vector<VertexNormal> & vN = {}, const std::vector<VertexTextureCoordinates> & vT = {});
public:
    virtual ~Mesh_Base();

    GLuint GetVerticesVAO() const;
    GLuint GetFacesVAO() const;
    GLuint GetVerticesVBO() const;
    GLuint GetFacesVBO() const;
    GLuint GetVerticesCount() const;
    GLuint GetFacesVerticesCount() const;

    virtual const std::vector<VertexPos> * GetVerticesPos() const;
    virtual const std::vector<VertexNormal> * GetVerticesNormals() const;
    virtual const std::vector<VertexTextureCoordinates> * GetVerticesTextureCoordinates() const;
    virtual const std::vector<Face> * GetFaces() const;

    template<Mesh_Based M>
    M * Cast();

    template<Mesh_Based M>
    const M * Cast() const;

    virtual GLuint GetFacesEBO() const = 0;
    virtual bool IsUsingEBO() const = 0;

    enum class DrawMode : unsigned char
    {
        DrawArrays = 0,
        DrawElements = 1
    };
    virtual DrawMode GetDrawMode() const = 0;

    bool HasTextureCoordinates() const;
    bool HasNormals() const;

    void GenerateNormals(bool smooth);
    void SetGeometry(const std::vector<Face> & faces, const std::vector<VertexPos> & v, const std::vector<VertexNormal> & vN = {}, const std::vector<VertexTextureCoordinates> & vT = {});

protected:
    void LoadVertices(const std::vector<VertexPos> & vertices);
    void LoadFaces(const std::vector<VertexNormalTexture> & vertices);
    std::vector<VertexNormalTexture> GenerateAssembledVertices(bool isNormal, bool isTexture) const;

protected:
    GLuint __verticesVAO, __facesVAO;
    GLuint __verticesVBO, __facesVBO;
    GLuint __verticesNVert, __facesNVert;
    bool __hasTextureCoordinates, __hasNormals;

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

#include "Mesh_Base.inl"