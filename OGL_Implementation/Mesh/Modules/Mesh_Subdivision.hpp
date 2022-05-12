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
#include "Mesh_ThreadPool.hpp"

// C++ includes
#include <map>

/**
 * @brief Manages Mesh Subdivision operations
*/
class Mesh_Subdivision
{
public:
    static Mesh_Custom * Subdivide(Mesh_Base & mesh);
    static void SubdivideParallel(Mesh_Base & mesh);
private:
};
