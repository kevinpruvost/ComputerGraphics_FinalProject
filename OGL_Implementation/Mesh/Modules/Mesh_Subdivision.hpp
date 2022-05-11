/*****************************************************************//**
 * \file   Mesh_Subdivision.hpp
 * \brief  Mesh Subdivision module
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 06 2022
 *********************************************************************/
#pragma once

// Project includes
#include "../Mesh_Custom.hpp"

// C++ includes
#include <map>

class Mesh_Subdivision
{
public:
    Mesh_Subdivision();
    ~Mesh_Subdivision();

private:

private:
    

protected:
    Mesh_Custom * __Subdivide(Mesh_Base & mesh);
};
