/*****************************************************************//**
 * \file   Particle_Spirals.cpp
 * \brief  Particle Sprials source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 15 2022
 *********************************************************************/
#include "Particle_Spirals.hpp"

Particle_Spiral::Particle_Spiral(const float lifeSpan, const glm::vec3 & pos_, const glm::vec3 & scale_, const glm::vec4 & color_)
    : Particle_Base(lifeSpan, pos_, scale_)
{
    color = color_;
}
