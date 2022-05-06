/*****************************************************************//**
 * \file   Entity_Geometry.cpp
 * \brief  Entity Geometry Source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 15 2022
 *********************************************************************/
#include "Entity_Geometry.hpp"

glm::mat4 GetModelMatrixNoRotationScale(const glm::vec3 & pos)
{
    // Transformation Matrix
    return {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {pos.x, pos.y, pos.z, 1.0f}
    };
}

glm::mat4 GetModelMatrixNoRotation(const glm::vec3 & pos, const glm::vec3 & scale)
{
    // Transformation Matrix
    glm::mat4 mat = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {pos.x, pos.y, pos.z, 1.0f}
    };
    // Scaling Matrix
    mat = glm::scale(mat, scale);
    return mat;
}

glm::mat4 GetModelMatrixNoScale(const glm::vec3 & pos, const glm::quat & quaternion)
{
    // Transformation Matrix
    glm::mat4 mat = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {pos.x, pos.y, pos.z, 1.0f}
    };
    // Rotation Matrix
    mat *= glm::toMat4(quaternion);
    return mat;
}

glm::mat4 GetModelMatrix(const glm::vec3 & pos, const glm::quat & quaternion, const glm::vec3 & scale)
{
    // Transformation Matrix
    glm::mat4 mat = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {pos.x, pos.y, pos.z, 1.0f}
    };
    // Rotation Matrix
    mat *= glm::toMat4(quaternion);
    // Scaling Matrix
    mat = glm::scale(mat, scale);
    return mat;
}

inline glm::vec3 GetLocalPosition(const glm::vec3 & pos)
{
    return pos;
}
