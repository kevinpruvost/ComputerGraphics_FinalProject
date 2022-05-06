/*****************************************************************//**
 * \file   Entity_Skeleton.cpp
 * \brief  Entity Skeleton source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 08 2022
 *********************************************************************/
#include "Entity_Skeleton.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"

Entity_Skeleton::Entity_Skeleton()
    : __parent{ nullptr }
{
}

Entity_Skeleton::~Entity_Skeleton()
{
    RemoveParent();
    RemoveChildren();
}

glm::vec3 Entity_Skeleton::GetWorldPosition() const
{
    if (HasParent())
    {
        const glm::mat4 modelMatrix = GetModelMatrix();
        const glm::vec3 position{ modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2] };
        return position;
    }
    return GetLocalPosition();
}

bool Entity_Skeleton::HasChildren() const
{
    return !__children.empty();
}

size_t Entity_Skeleton::ChildrenCount() const
{
    return __children.size();
}

bool Entity_Skeleton::HasParent() const
{
    return __parent != nullptr;
}

void Entity_Skeleton::RemoveParent()
{
    if (__parent)
    {
        __parent->RemoveChild(this);
        __parent = nullptr;
    }
}

void Entity_Skeleton::RemoveChildren()
{
    if (!__children.empty())
    {
        for (auto * child : __children)
        {
            child->RemoveParent();
        }
        __children.clear();
    }
}

const Entity_Skeleton * Entity_Skeleton::ToSkeleton() const
{
    return dynamic_cast<const Entity_Skeleton *>(this);
}

Entity_Skeleton * Entity_Skeleton::ToSkeleton()
{
    return dynamic_cast<Entity_Skeleton *>(this);
}

const std::vector<Entity_Skeleton *> & Entity_Skeleton::GetChildren() const
{
    return __children;
}
