/*****************************************************************//**
 * \file   Particle_Base.inl
 * \brief  Implementation of Particle_Base templated code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 15 2022
 *********************************************************************/
#pragma once

#include "Particle_Base.hpp"

template<Particle_Based PType>
inline const PType * Particle_Base::Cast() const
{
    return dynamic_cast<const PType *>(this);
}

template<Particle_Based PType>
inline PType * Particle_Base::Cast()
{
    return dynamic_cast<PType *>(this);
}
