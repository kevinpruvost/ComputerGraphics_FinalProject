/*****************************************************************//**
 * \file   AEntity.cpp
 * \brief  Entity Skeleton source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 08 2022
 *********************************************************************/
#include "AEntity.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"

AEntity::AEntity()
    : __parent{ nullptr }
{
}

AEntity::~AEntity()
{
    RemoveParent();
    RemoveChildren();
}

glm::vec3 AEntity::GetWorldPosition() const
{
    if (HasParent())
    {
        const glm::mat4 modelMatrix = GetModelMatrix();
        const glm::vec3 position{ modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2] };
        return position;
    }
    return GetLocalPosition();
}

bool AEntity::HasChildren() const
{
    return !__children.empty();
}

size_t AEntity::ChildrenCount() const
{
    return __children.size();
}

bool AEntity::HasParent() const
{
    return __parent != nullptr;
}

void AEntity::RemoveParent()
{
    if (__parent)
    {
        __parent->RemoveChild(this);
        __parent = nullptr;
    }
}

void AEntity::RemoveChildren()
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

const AEntity * AEntity::ToSkeleton() const
{
    return dynamic_cast<const AEntity *>(this);
}

AEntity * AEntity::ToSkeleton()
{
    return dynamic_cast<AEntity *>(this);
}

const std::vector<AEntity *> & AEntity::GetChildren() const
{
    return __children;
}
