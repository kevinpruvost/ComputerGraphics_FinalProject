/*****************************************************************//**
 * \file   AEntity.hpp
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

class AEntity;

template<class T>
/// @brief Concept checking if the type is an integral or a floating point.
concept AEntity_Based = std::is_base_of_v<AEntity, T>;

/**
 * @brief Base for all entities
*/
class AEntity
{
protected:
    AEntity();
public:
    virtual ~AEntity();

    virtual glm::mat4 GetModelMatrix(bool ignoreRotation = false, bool ignoreScale = false) const = 0;
    virtual glm::vec3 GetLocalPosition() const = 0;
    glm::vec3 GetWorldPosition() const;

public:
    template<AEntity_Based E>
    void SetParent(E & parent);

    template<AEntity_Based Parent = AEntity>
    const Parent * GetParent() const;

    template<AEntity_Based Parent = AEntity>
    Parent * GetParent();

    template<AEntity_Based Child = AEntity>
    void AddChild(const Child * child);

    template<AEntity_Based Child = AEntity>
    void RemoveChild(const Child * child);

    template<AEntity_Based Child = AEntity>
    bool HasChild(const Child * child) const;
    bool HasChildren() const;
    size_t ChildrenCount() const;
    bool HasParent() const;
    void RemoveParent();
    void RemoveChildren();

    const AEntity * ToSkeleton() const;
    AEntity * ToSkeleton();

    const std::vector<AEntity *> & GetChildren() const;
    
private:
    AEntity * __parent;
    std::vector<AEntity *> __children;
};

#include "AEntity.inl"