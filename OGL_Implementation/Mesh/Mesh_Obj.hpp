/*****************************************************************//**
 * \file   Mesh_Obj.hpp
 * \brief  Mesh created from Obj class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Mesh_Base.hpp"
#include "OGL_Implementation\Obj.hpp"

/**
 * @brief Mesh constructed from the Obj class, with the help of .obj files.
*/
class Mesh_Obj : public Mesh_Base
{
public:
    /**
     * @brief Constructor from .obj class
     * @param obj
    */
    Mesh_Obj(const Obj & obj);
    ~Mesh_Obj();

    GLuint GetFacesEBO() const override;
    bool IsUsingEBO() const override;

    /**
     * @brief Returns draw mode (glDrawArrays/glDrawElements)
     * @return draw mode
    */
    DrawMode GetDrawMode() const override;

private:
    void bindFaces(const Obj & obj);
    void bindVertices(const Obj & obj);
};
