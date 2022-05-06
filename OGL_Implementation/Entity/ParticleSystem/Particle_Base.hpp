/*****************************************************************//**
 * \file   Particle_Base.hpp
 * \brief  Particle Base class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 15 2022
 *********************************************************************/
#pragma once

// Project includes
#include "../Entity.hpp"

// GLM includes
#include <GLM\glm.hpp>

class Particle_Base;

/// @brief Concept checking if the type is based on particle class
template<class T>
concept Particle_Based = std::is_base_of_v<Particle_Base, T>;

/**
 * @brief Base class for all particles
*/
class Particle_Base : public Entity_Skeleton
{
public:
    Particle_Base(const float lifeSpan, const glm::vec3 & pos_, const glm::vec3 & scale_);
    ~Particle_Base();

    virtual glm::mat4 GetModelMatrix(bool ignoreRotation = false, bool ignoreScale = false) const;
    virtual glm::vec3 GetLocalPosition() const;

    /**
     * @brief Updates Life Span for deltaTime seconds
     * @param deltaTime 
    */
    void UpdateLifeSpan(const float deltaTime);
    /**
     * @brief Gets remaining life span
     * @return lifeSpan
    */
    float GetLifeSpan() const;

    /**
     * @brief Converts Particle based class to Particle_Base (const)
     * @return ptr to self
    */
    const Particle_Base * ToBase() const;
    /**
     * @brief Converts Particle based class to Particle_Base (mutable)
     * @return ptr to self
    */
    Particle_Base * ToBase();

    /**
     * @brief Cast to a Particle based class (const)
     * @return ptr to self
    */
    template<Particle_Based PType>
    const PType * Cast() const;

    /**
     * @brief Cast to a Particle based class (mutable)
     * @return ptr to self
    */
    template<Particle_Based PType>
    PType * Cast();

    /**
     * @brief Sets all movement variables
     * @param direction
     * @param speed
     * @param acceleration
    */
    void SetMovement(const glm::vec3 & speed_, const glm::vec3 & acceleration_);

public:
    /**
     * @brief 3D position
    */
    glm::vec3 pos;
    /**
     * @brief 3D Scale
    */
    const glm::vec3 scale;

    /**
     * @brief Speed (X,Y,Z)
    */
    glm::vec3 speed;
    /**
     * @brief Acceleration (X,Y,Z)
    */
    glm::vec3 acceleration;

public:
    /**
     * @brief Color (RGBA)
    */
    glm::vec4 color;

    /**
     * @brief For further properties specialization (for GLSL)
    */
    enum class ParticlePropertiesStyle : unsigned int
    {
        Color = 1 << 0,
    };

private:
    float __lifeSpan;
};

#include "Particle_Base.inl"
