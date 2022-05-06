/*****************************************************************//**
 * \file   Tools.hpp
 * \brief  Tools header
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 28 2022
 *********************************************************************/
#pragma once

// C++ includes
#include <random>

inline float RandomFloat(const float min, const float max, const unsigned int step)
{
    return min + (max - min) / (step + 1) * static_cast<float>(rand() % (step + 2));
}