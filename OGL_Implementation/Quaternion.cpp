/*****************************************************************//**
 * \file   Quaternion.cpp
 * \brief  Quaternion source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 10 2022
 *********************************************************************/
#include "Quaternion.hpp"

// C++ includes
#include <numbers>
#include <algorithm>

Quaternion::Quaternion(const glm::vec3 & rotation)
    : glm::quat(glm::radians(rotation))
    , __eulerAngles{ rotation }
{
}

Quaternion::~Quaternion()
{
}

Quaternion & Quaternion::operator=(const glm::quat & quat)
{
    *(dynamic_cast<glm::quat *>(this)) = quat;
    return *this;
}

Quaternion::operator glm::quat & ()
{
    return *(dynamic_cast<glm::quat *>(this));
}

Quaternion::operator const glm::quat & () const
{
    return *(dynamic_cast<const glm::quat *>(this));
}

#include "OGL_Implementation\DebugInfo\Log.hpp"
#include <glm\gtx\string_cast.hpp>
void Quaternion::SetRotation(const glm::vec3 & rotation)
{
    *this = glm::quat(glm::radians(rotation));
    __eulerAngles = rotation;
}

void Quaternion::Rotate(const glm::vec3 & rotation)
{
    LOG_PRINT(stdout, "Rotation: %s\n", glm::to_string(rotation).c_str());
    const float rot = std::max({ abs(rotation.x), abs(rotation.y), abs(rotation.z) });
    if (rot == 0.0f) return;
    const glm::vec3 axes = rotation / rot;
    LOG_PRINT(stdout, "Rot: %.2f\nAxes: %s\n", rot, glm::to_string(axes).c_str());
    *this = glm::rotate(*this, glm::radians(rot), axes);
    __eulerAngles += rotation;
    __eulerAngles.x = std::fmod(__eulerAngles.x, 360.0f - std::numeric_limits<float>::epsilon());
    __eulerAngles.y = std::fmod(__eulerAngles.y, 360.0f - std::numeric_limits<float>::epsilon());
    __eulerAngles.z = std::fmod(__eulerAngles.z, 360.0f - std::numeric_limits<float>::epsilon());
}

void Quaternion::RotateX(const float rotation)
{
    constexpr const glm::vec3 X(1.0f, 0.0f, 0.0f);
    *this = glm::rotate(*this, glm::radians(std::fmod(rotation, 360.0f)), X);
    __eulerAngles.x = std::fmod(__eulerAngles.x + rotation, 360.0f - std::numeric_limits<float>::epsilon());
}

void Quaternion::RotateY(const float rotation)
{
    constexpr const glm::vec3 Y(0.0f, 1.0f, 0.0f);
    *this = glm::rotate(*this, glm::radians(std::fmod(rotation, 360.0f)), Y);
    __eulerAngles.y = std::fmod(__eulerAngles.y + rotation, 360.0f - std::numeric_limits<float>::epsilon());
}

void Quaternion::RotateZ(const float rotation)
{
    constexpr const glm::vec3 Z(0.0f, 0.0f, 1.0f);
    *this = glm::rotate(*this, glm::radians(std::fmod(rotation, 360.0f)), Z);
    __eulerAngles.z = std::fmod(__eulerAngles.z + rotation, 360.0f - std::numeric_limits<float>::epsilon());
}

glm::vec3 Quaternion::ToEulerAngles() const
{
    return __eulerAngles;
}