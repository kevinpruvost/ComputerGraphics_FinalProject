/*****************************************************************//**
 * \file   ParticleSystemRendering.hpp
 * \brief  Particle System Rendering class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 15 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Entity\ParticleSystem\ParticleSystem.hpp"
#include "RenderingSettings.hpp"

// GLAD includes
#include <GLAD\glad.h>

/**
 * @brief Rendering specialized for particle systems drawing.
*/
class ParticleSystemRendering
{
public:
    ParticleSystemRendering();
    ~ParticleSystemRendering();

    GLuint GetVAO();
    GLuint GetVBO();

    static void Init();
    static const ParticleSystemRendering & Get();
    static GLuint GetParticleSystemRenderingVAO();
    static GLuint GetParticleSystemRenderingVBO();

    static void DrawParticleSystem(ParticleSystem_Base * particleSystem);

private:
    GLuint __vao, __vbo;
};
