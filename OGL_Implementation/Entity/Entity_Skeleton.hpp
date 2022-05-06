/*****************************************************************//**
 * \file   Entity_Skeleton.hpp
 * \brief  Entity Skeleton code, base of all entities
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 08 2022
 *********************************************************************/
#pragma once

// GLM includes
#include <GLM\glm.hpp>

// C++ includes
#include <type_traits>
#include <vector>

class Entity_Skeleton;

template<class T>
/// @brief Concept checking if the type is an integral or a floating point.
concept Entity_Skeleton_Based = std::is_base_of_v<Entity_Skeleton, T>;

/**
 * @brief Basis for all entities
*/
class Entity_Skeleton
{
protected:
    Entity_Skeleton();
public:
    virtual ~Entity_Skeleton();

    virtual glm::mat4 GetModelMatrix(bool ignoreRotation = false, bool ignoreScale = false) const = 0;
    virtual glm::vec3 GetLocalPosition() const = 0;
    glm::vec3 GetWorldPosition() const;

public:
    template<Entity_Skeleton_Based E>
    void SetParent(E & parent);

    template<Entity_Skeleton_Based Parent = Entity_Skeleton>
    const Parent * GetParent() const;

    template<Entity_Skeleton_Based Parent = Entity_Skeleton>
    Parent * GetParent();

    template<Entity_Skeleton_Based Child = Entity_Skeleton>
    void AddChild(const Child * child);

    template<Entity_Skeleton_Based Child = Entity_Skeleton>
    void RemoveChild(const Child * child);

    template<Entity_Skeleton_Based Child = Entity_Skeleton>
    bool HasChild(const Child * child) const;
    bool HasChildren() const;
    size_t ChildrenCount() const;
    bool HasParent() const;
    void RemoveParent();
    void RemoveChildren();

    const Entity_Skeleton * ToSkeleton() const;
    Entity_Skeleton * ToSkeleton();

    const std::vector<Entity_Skeleton *> & GetChildren() const;
    
private:
    Entity_Skeleton * __parent;
    std::vector<Entity_Skeleton *> __children;
};

#include "Entity_Skeleton.inl"