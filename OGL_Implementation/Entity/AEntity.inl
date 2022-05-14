/*****************************************************************//**
 * \file   AEntity.inl
 * \brief  Entity Skeleton inline/template code
 *
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 08 2022
 *********************************************************************/
#include "AEntity.hpp"

// C++ includes
#include <algorithm>

template<AEntity_Based E>
inline void AEntity::SetParent(E & parent)
{
    __parent = dynamic_cast<AEntity *>(&parent);
}

template<AEntity_Based Parent>
inline const Parent * AEntity::GetParent() const
{
    return __parent ? dynamic_cast<const Parent *>(__parent) : nullptr;
}

template<AEntity_Based Parent>
inline Parent * AEntity::GetParent()
{
    return __parent ? dynamic_cast<Parent *>(__parent) : nullptr;
}

template<AEntity_Based Child>
inline void AEntity::AddChild(const Child * child)
{
    __children.emplace_back(dynamic_cast<const AEntity *>(child));
}

template<AEntity_Based Child>
inline void AEntity::RemoveChild(const Child * child)
{
    if (HasChild(child))
    {
        const auto ite = std::find(__children.begin(), __children.end(), dynamic_cast<const AEntity *>(child));
        __children.erase(ite);
    }
}

template<AEntity_Based Child>
inline bool AEntity::HasChild(const Child * child) const
{
    return std::find(__children.begin(), __children.end(), dynamic_cast<const AEntity *>(child)) != __children.end();
}
