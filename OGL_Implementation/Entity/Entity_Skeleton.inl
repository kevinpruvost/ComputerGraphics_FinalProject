/*****************************************************************//**
 * \file   Entity_Skeleton.inl
 * \brief  Entity Skeleton inline/template code
 *
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 08 2022
 *********************************************************************/
#include "Entity_Skeleton.hpp"

// C++ includes
#include <algorithm>

template<Entity_Skeleton_Based E>
inline void Entity_Skeleton::SetParent(E & parent)
{
    __parent = dynamic_cast<Entity_Skeleton *>(&parent);
}

template<Entity_Skeleton_Based Parent>
inline const Parent * Entity_Skeleton::GetParent() const
{
    return __parent ? dynamic_cast<const Parent *>(__parent) : nullptr;
}

template<Entity_Skeleton_Based Parent>
inline Parent * Entity_Skeleton::GetParent()
{
    return __parent ? dynamic_cast<Parent *>(__parent) : nullptr;
}

template<Entity_Skeleton_Based Child>
inline void Entity_Skeleton::AddChild(const Child * child)
{
    __children.emplace_back(dynamic_cast<const Entity_Skeleton *>(child));
}

template<Entity_Skeleton_Based Child>
inline void Entity_Skeleton::RemoveChild(const Child * child)
{
    if (HasChild(child))
    {
        const auto ite = std::find(__children.begin(), __children.end(), dynamic_cast<const Entity_Skeleton *>(child));
        __children.erase(ite);
    }
}

template<Entity_Skeleton_Based Child>
inline bool Entity_Skeleton::HasChild(const Child * child) const
{
    return std::find(__children.begin(), __children.end(), dynamic_cast<const Entity_Skeleton *>(child)) != __children.end();
}
