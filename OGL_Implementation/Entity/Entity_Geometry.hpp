/*****************************************************************//**
 * \file   Entity_Geometry.hpp
 * \brief  Entity Geometry header
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 15 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Entity_Skeleton.hpp"

// GLM includes
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>

glm::mat4 GetModelMatrixNoRotationScale(const glm::vec3 & pos);
glm::mat4 GetModelMatrixNoRotation(const glm::vec3 & pos, const glm::vec3 & scale);
glm::mat4 GetModelMatrixNoScale(const glm::vec3 & pos, const glm::quat & quaternion);
glm::mat4 GetModelMatrix(const glm::vec3 & pos, const glm::quat & quaternion, const glm::vec3 & scale);
inline glm::vec3 GetLocalPosition(const glm::vec3 & pos);
