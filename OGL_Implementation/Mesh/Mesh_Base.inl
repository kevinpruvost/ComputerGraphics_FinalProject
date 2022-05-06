/*****************************************************************//**
 * \file   Mesh_Base.inl
 * \brief  Mesh_Base inline code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   May, 01 2022
 *********************************************************************/
#include "Mesh_Base.hpp"

template<Mesh_Based M>
inline const M * Mesh_Base::Cast() const
{
    return dynamic_cast<const M *>(this);
}

template<Mesh_Based M>
inline M * Mesh_Base::Cast()
{
    return dynamic_cast<M *>(this);
}