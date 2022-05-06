/*****************************************************************//**
 * \file   Quaternion.hpp
 * \brief  Quaternion class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 10 2022
 *********************************************************************/
#pragma once

// GLM includes
#include <GLM\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\quaternion.hpp>

/**
 * @brief Quaternion structure, serves for rotation purposes.
 * Almost every inputs user-side are expressed in degrees from the Yaw Pitch Roll coordinate system.
*/
class Quaternion : public glm::quat
{
public:
    /**
     * @brief Quaternion constructor with rotation in degrees
     * @param rotation 
    */
    Quaternion(const glm::vec3 & rotation = { 0.0f, 0.0f, 0.0f });
    ~Quaternion();
    Quaternion & operator=(const glm::quat & quat);
    operator glm::quat & ();
    operator const glm::quat & () const;

    /**
     * @brief Sets degrees rotation
     * @param rotation 
    */
    void SetRotation(const glm::vec3 & rotation);
    /**
     * @brief Rotates on X, Y, Z axis (degrees)
     * @param rotation 
    */
    void Rotate(const glm::vec3 & rotation);
    /**
     * @brief Rotates on X axis (degrees)
     * @param rotation 
    */
    void RotateX(const float rotation);
    /**
     * @brief Rotates on Y axis (degrees)
     * @param rotation 
    */
    void RotateY(const float rotation);
    /**
     * @brief Rotates on Z axis (degrees)
     * @param rotation 
    */
    void RotateZ(const float rotation);
    /**
     * @brief Returns Euler Angles
     * https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_angles_conversion
     * @return euler angles
    */
    glm::vec3 ToEulerAngles() const;

private:
    glm::vec3 __eulerAngles;
};
