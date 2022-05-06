/*****************************************************************//**
 * \file   ParticleSystem_Snows.cpp
 * \brief  ParticleSystem Snows source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 26 2022
 *********************************************************************/
#include "ParticleSystem_Snow.hpp"

// Project includes
#include "OGL_Implementation\Window.hpp"
#include "OGL_Implementation\Tools\Tools.hpp"

// GLM includes
#include <glm/gtx/string_cast.hpp>

ParticleSystem_Snow::ParticleSystem_Snow(const Shader & shaderPoint_, const Shader & shaderWireframe_, const Shader & shaderPS_)
    : ParticleSystem_Base(shaderPoint_, shaderWireframe_, shaderPS_, 10.0f, 5.0f, 50)
    , particleSpeed{ 1.0f }
    , __theta{ 0.0f }
    , gravity{ 9.8f }
    , radius{ 1.0f }
    , minScale{ 0.5f }
    , maxScale{ 2.0f }
    , stepScale{ 5 }
{
    __particles.reserve(maxParticles);
}

ParticleSystem_Snow::~ParticleSystem_Snow()
{
}

void ParticleSystem_Snow::OnUpdate()
{
}

void ParticleSystem_Snow::UpdateParticle(Particle_Base * particle_)
{
    Particle_Snow & particle = *particle_->Cast<Particle_Snow>();

    particle.speed.y = particle.speed.y - _deltaTime * gravity;
    if (particle.speed.y <= -gravity)
        particle.speed.y = -gravity;
    particle.pos += particle.speed * _deltaTime;
}

std::vector<Particle_Base *> ParticleSystem_Snow::SpawnParticle()
{
    std::vector<Particle_Base *> newParticles;
    Particle_Snow * particle = new Particle_Snow(lifeSpan,
        pos + glm::vec3(RandomFloat(-radius, radius, 50), 0.0, 0.0),
        glm::vec3{RandomFloat(minScale, maxScale, stepScale)},
        glm::vec4{ 1.0f }
    );

    newParticles.push_back(particle);
    return newParticles;
}

void ParticleSystem_Snow::OnStart()
{
}

void ParticleSystem_Snow::OnStop()
{
}

void ParticleSystem_Snow::OnReset()
{
    __theta = 0.0f;
}

Particle_Base::ParticlePropertiesStyle ParticleSystem_Snow::GetParticlePropertiesStyle() const
{
    return Particle_Base::ParticlePropertiesStyle::Color;
}