/*****************************************************************//**
 * \file   ParticleSystem.hpp
 * \brief  ParticleSystem class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 14 2022
 *********************************************************************/
#pragma once

// Project includes
#include "..\Entity.hpp"
#include "Particle_Base.hpp"
#include "OGL_Implementation\Rendering\RenderingSettings.hpp"

/**
 * @brief Manages the whole particle systems animation & overall process.
 * Inherits Entity
*/
class ParticleSystem_Base : public Entity_Skeleton
{
public:
    ParticleSystem_Base(const Shader & shaderPoint_, const Shader & shaderWireframe_, const Shader & shaderPS_, const float lifeSpan_, const float frequency_, const unsigned int maxParticles_);
    ~ParticleSystem_Base();

    virtual glm::mat4 GetModelMatrix(bool ignoreRotation = false, bool ignoreScale = false) const;
    virtual glm::vec3 GetLocalPosition() const;
    virtual glm::vec3 GetWorldPosition() const;

    virtual Particle_Base::ParticlePropertiesStyle GetParticlePropertiesStyle() const = 0;

    void Update();

protected:
    inline void RemoveParticle();
    virtual void UpdateParticle(Particle_Base * particle) = 0;
    virtual std::vector<Particle_Base *> SpawnParticle() = 0;

public:
    /// @brief Starts particle emission
    void Start();
    /// @brief Resets particle emission
    void Reset();
    /// @brief Stops particle emission
    void Stop();

    /**
     * @brief Returns if particle emission is stopped
     * @return stopped
    */
    bool isStopped() const;

    /**
     * @brief Virtual Callback called at the start of Update(),
     * to rewrite in children classes
    */
    virtual void OnUpdate();
    /**
     * @brief Virtual Callback called at the start of Start(),
     * to rewrite in children classes
    */
    virtual void OnStart();
    /**
     * @brief Virtual Callback called at the start of Stop(),
     * to rewrite in children classes
    */
    virtual void OnStop();
    /**
     * @brief Virtual Callback called at the start of Reset(),
     * to rewrite in children classes
    */
    virtual void OnReset();

    /**
     * @brief Returns every particle currently emitted
     * @return particles
    */
    const std::vector<std::unique_ptr<Particle_Base>> & GetParticles() const;

    /**
     * @brief Cast to ParticleSystem_Base (const)
     * @return ptr to self
    */
    const ParticleSystem_Base * ToParticleSystemBase() const;

    /**
     * @brief Cast to ParticleSystem_Base (mutable)
     * @return ptr to self
    */
    ParticleSystem_Base * ToParticleSystemBase();

protected:
    inline void ResetCounter();

public:
    /**
     * @brief Transformation Vector.
    */
    glm::vec3 pos;
    /**
     * @brief Scaling Vector.
    */
    glm::vec3 scale;
    /**
     * @brief Quaternion
    */
    Quaternion quat;

    /// @brief Shader used to draw particles
    Shader shaderPoint, shaderWireframe, shaderParticleSystem;
    RenderingMode displayMode;
    Texture texture;

    // @brief Life Span (in seconds)
    float lifeSpan;
    /// @brief Frequency of particle spawn (particles/second)
    float frequency;
    /// @brief Max number of particles
    unsigned int maxParticles;

protected:
    std::vector<std::unique_ptr<Particle_Base>> __particles;

    float _deltaTime;
private:
    bool __turnedOn;
    float __deltaTimeCounter;
    std::vector<std::unique_ptr<Particle_Base>>::iterator __particleIte;
};
