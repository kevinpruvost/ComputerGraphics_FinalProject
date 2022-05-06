/*****************************************************************//**
 * \file   ParticleSystemGeometry.hpp
 * \brief  ParticleSystem Geometry related calculations
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 15 2022
 *********************************************************************/
#pragma once

// GLM includes
#include <GLM\glm.hpp>

/**
 * @brief Calculates (x, y, z) position considering polar coordinates and a & b factors
 * with Archimedes Spiral formula.
 * @param theta 
 * @param a 
 * @param b 
 * @return vec3 (x, y, z) position
*/
glm::vec3 CalculateArchimedesSpiral(const float theta, const float a, const float b);

/**
 * @brief 
 * @param theta 
 * @param a 
 * @param b 
 * @return 
*/
glm::vec3 CalculateFermatSpiral(const float theta, const float a);

/**
 * @brief 
 * @param theta 
 * @param a 
 * @param b 
 * @return 
*/
glm::vec3 CalculateLogSpiral(const float theta, const float a, const float b);
