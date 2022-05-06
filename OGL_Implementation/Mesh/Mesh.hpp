/*****************************************************************//**
 * \file   Mesh.hpp
 * \brief  Mesh class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 27 2022
 *********************************************************************/
#pragma once

// C++ includes
#include <memory>

// Project includes
#include "Mesh_Base.hpp"
#include "Mesh_Obj.hpp"
#include "Mesh_Sphere.hpp"
#include "Mesh_Image.hpp"
#include "Mesh_Custom.hpp"

#include "OGL_Implementation\DebugInfo\Log.hpp"

/**
 * @brief Contains Mesh Id and methods related
 * to the mesh database.
 * It also helps having a much lighter class because Mesh_Base weighs 30 bytes
 * and Mesh only 2 bytes.
 * And most importantly, the usage is way easier, because copies, constructors, destructors,
 * will not affect the original mesh
*/
class Mesh
{
public:
    /**
     * @brief Constructor from Mesh ID.
     * @param meshId
    */
    Mesh(const GLuint meshId);
    Mesh & operator=(const Mesh & mesh);

public:
    GLuint meshId() const;

    GLuint verticesVAO() const;
    GLuint facesVAO() const;
    GLuint verticesVBO() const;
    GLuint facesVBO() const;
    GLuint verticesNVert() const;
    GLuint facesNVert() const;

    Mesh_Base * operator*();
    const Mesh_Base * operator*() const;

    GLuint facesEBO() const;
    bool isUsingEBO() const;

    Mesh_Base::DrawMode GetDrawMode() const;

private:
    GLuint __meshId;
};

/**
 * @brief Generates mesh from image parameters
 * @return mesh
*/
Mesh GenerateMeshImage();
/**
 * @brief Generates mesh from sphere parameters
 * @param radius 
 * @param sectorCount 
 * @param stackCount 
 * @param smooth 
 * @return mesh
*/
Mesh GenerateMeshSphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18, bool smooth = true);
/**
 * @brief Generates mesh from vertices list
 * @param vertices
 * @return mesh
*/
Mesh GenerateMesh(const std::vector<VertexNormalTexture> & vertices);
/**
 * @brief Generates mesh from obj class, stores it in the mesh database
 * and returns a Mesh class.
 * @param obj 
 * @return mesh
*/
Mesh GenerateMesh(const Obj & obj);
/**
 * @brief Generates mesh from mesh
 * @param mesh
 * @return mesh
*/
Mesh GenerateMesh(const Mesh & mesh);
/**
 * @brief Generates mesh from mesh id
 * @param meshId 
 * @return mesh
*/
Mesh GenerateMesh(const uint16_t meshId);