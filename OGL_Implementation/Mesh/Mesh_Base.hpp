/*****************************************************************//**
 * \file   Mesh_Base.hpp
 * \brief  Mesh_Base class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#pragma once

// GLAD includes
#include <GLAD\glad.h>

// C++ includes
#include <type_traits>

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
public:
    virtual ~Mesh_Base();

    GLuint GetVerticesVAO() const;
    GLuint GetFacesVAO() const;
    GLuint GetVerticesVBO() const;
    GLuint GetFacesVBO() const;
    GLuint GetVerticesCount() const;
    GLuint GetFacesVerticesCount() const;

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

private:

protected:
    GLuint __verticesVAO, __facesVAO;
    GLuint __verticesVBO, __facesVBO;
    GLuint __verticesNVert, __facesNVert;
    bool __hasTextureCoordinates, __hasNormals;
};

#include "Mesh_Base.inl"