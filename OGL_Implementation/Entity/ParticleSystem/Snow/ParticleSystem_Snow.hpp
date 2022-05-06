/*****************************************************************//**
 * \file   ParticleSystem_Snow.hpp
 * \brief  Particle System Snows class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 26 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Particle_Snow.hpp"
#include "../ParticleSystem_Base.hpp"
#include "../ParticleSystemGeometry.hpp"

/**
 * @brief Particle System based on Snows (Archimedes, Fermat & Logarithmic)
*/
class ParticleSystem_Snow : public ParticleSystem_Base
{
public:
    ParticleSystem_Snow(const Shader & shaderPoint_, const Shader & shaderWireframe_, const Shader & shaderPS_);
    ~ParticleSystem_Snow();

    void UpdateParticle(Particle_Base * particle) override;
    std::vector<Particle_Base *> SpawnParticle() override;

    void OnUpdate() override;
    void OnStart() override;
    void OnStop() override;
    void OnReset() override;

    Particle_Base::ParticlePropertiesStyle GetParticlePropertiesStyle() const;

private:

public:
    /// @brief Particle
    float particleSpeed;
    /// @brief Gravity
    float gravity;
    /// @brief Radius (area covered by the snow spawn)
    float radius;
    /// @brief Minimum Scale
    float minScale;
    /// @brief Maximum Scale
    float maxScale;
    /// @brief Steps between different scales (e.g stepScale(2) => [min, min + ((max-min) / (stepScale + 1)) * 1, min + ((max-min) / (stepScale + 1)) * 2, max]
    unsigned int stepScale;

private:
    float __theta;
};
