/*****************************************************************//**
 * \file   Mesh_Image.hpp
 * \brief  Mesh of Image
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 28 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Mesh_Base.hpp"

/**
 * @brief Mesh formed to display images
*/
class Mesh_Image : public Mesh_Base
{
public:
    Mesh_Image();
    ~Mesh_Image();

    GLuint GetFacesEBO() const;
    bool IsUsingEBO() const;
    Mesh_Base::DrawMode GetDrawMode() const;
};
