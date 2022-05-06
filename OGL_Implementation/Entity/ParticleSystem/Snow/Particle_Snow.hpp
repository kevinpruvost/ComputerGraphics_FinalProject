/*****************************************************************//**
 * \file   Particle_Spirals.hpp
 * \brief  Particle Spirals class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 26 2022
 *********************************************************************/
#pragma once

#include "../Particle_Base.hpp"

/**
 * @brief Particle specialized for spiral particle systems
*/
class Particle_Snow : public Particle_Base
{
public:
    Particle_Snow(const float lifeSpan, const glm::vec3 & pos_, const glm::vec3 & scale_, const glm::vec4 & color_);
};
