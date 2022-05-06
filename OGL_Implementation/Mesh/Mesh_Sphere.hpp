/*****************************************************************//**
 * \file   Mesh_Sphere.hpp
 * \brief  Mesh_Sphere class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Mesh_Base.hpp"
#include "Mesh_Geometry.hpp"

// C++ includes
#include <vector>

/**
 * @brief Sphere mesh cosntructed from its parameters.
*/
class Mesh_Sphere : public Mesh_Base
{
public:
    /**
     * @brief Default Sphere Constructor
     * @param radius
     * @param sectorCount 
     * @param stackCount 
     * @param smooth 
    */
    Mesh_Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18, bool smooth = true);
    ~Mesh_Sphere();

    GLuint GetFacesEBO() const override;
    bool IsUsingEBO() const override;

private:
    void buildVertices(std::vector<Vertex> & vertices, std::vector<VertexNormalTexture> & vnts, bool calculateNormal = true);
    void bind3SizedVertices(const std::vector<Vertex> & vertices);
    void bindVnts(const std::vector<VertexNormalTexture> & vnts, const std::vector<GLuint> & indices);

    void buildVerticesSmooth();
    void buildVerticesFlat();

    DrawMode GetDrawMode() const override;

private:
    float __radius;
    int __sectors, __stacks;
    bool __smooth;

    GLuint __facesEBO;
};
