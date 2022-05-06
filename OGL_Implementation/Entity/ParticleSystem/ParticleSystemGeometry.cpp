/*****************************************************************//**
 * \file   ParticleSystemGeometry.cpp
 * \brief  ParticleSystemGeometry source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 15 2022
 *********************************************************************/
#include "ParticleSystemGeometry.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"

// GLM includes
#include <glm/gtx/polar_coordinates.hpp>

glm::vec3 CalculateArchimedesSpiral(const float theta, const float a, const float b)
{
    glm::vec2 polar{ a + theta * b, theta };
    return {polar.x * cos(polar.y), polar.x * sin(polar.y), 0.0f };
}

glm::vec3 CalculateFermatSpiral(const float theta, const float a)
{
    assert(theta >= 0.0f);
    glm::vec2 polar{ a * sqrt(theta), theta };
    return { polar.x * cos(polar.y), polar.x * sin(polar.y), 0.0f };
}

glm::vec3 CalculateLogSpiral(const float theta, const float a, const float b)
{
    glm::vec2 polar{ a * pow(b, theta), theta };
    return { polar.x * cos(polar.y), polar.x * sin(polar.y), 0.0f };
}